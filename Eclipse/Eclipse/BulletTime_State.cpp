#include "BulletTime_State.h"
#include "Attack_State.h"
#include "Fall_State.h"
#include "MovementFSM.h"
#include "PlayerFSM.h"
#include "PlayerAnimatorController.h"
#include "AfterImage.h"

#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/RenderSystem.h"
#include "../Direct2D_EngineLib/Vector2.h"
#include "../Direct2D_EngineLib/Camera.h"



void BulletTime_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[BulletTime_State] (( 진입 - 불릿 타임 시작 )) \n");
    Time::SetTimeScale(0.3f); // 시간 느리게

    fsm->GetPlayerFSM()->timer = 0.0f;

    blackOut_renderer = GameObject::Find("BlackOut")->GetComponent<SpriteRenderer>();
    blackOut_renderer->SetEnabled(true);
}

void BulletTime_State::Update(MovementFSM* fsm)
{
    float delta = Time::GetDeltaTime();
    float unscaledDelta = delta / Time::GetTimeScale();  // timeScale 나눠서 실제 시간 계산

    fsm->GetPlayerFSM()->timer += unscaledDelta;

    // [ Attack ] 마우스 왼쪽 버튼에서 손을 뗐을 때 -> 공격
    if (fsm->GetPlayerFSM()->GetIsLButtonUp())
    {
        Time::SetTimeScale(1.0f); 
        fsm->GetPlayerFSM()->OnAirAttack();
        fsm->GetPlayerFSM()->isBulletAttack = true; 
        fsm->ChangeState(std::make_unique<Attack_State>());
        return;
    }

    // [ Idle ] 공격 취소 
    if (fsm->GetPlayerFSM()->GetIsRButton())
    { 
        // 만약 마우스 왼쪽 누르고 있었으면 무시상태  
        if (fsm->GetPlayerFSM()->GetIsLButton()) fsm->GetPlayerFSM()->isAttackIgnore = true;

        if (fsm->GetPlayerFSM()->GetIsGround()) { fsm->ChangeState(std::make_unique<Idle_State>()); return; }
        else
        {
            fsm->ChangeState(std::make_unique<Fall_State>()); return;
        }
    }

    // [ Idle ] 불릿 타임 끝 
    if (fsm->GetPlayerFSM()->timer >= fsm->GetPlayerFSM()->bulletTimeDuration)
    {
        Time::SetTimeScale(1.0f); // 시간 복구

        // 만약 마우스 왼쪽 누르고 있었으면 무시상태  
        if (fsm->GetPlayerFSM()->GetIsLButton()) fsm->GetPlayerFSM()->isAttackIgnore = true;

        if (fsm->GetPlayerFSM()->GetIsGround()) { fsm->ChangeState(std::make_unique<Idle_State>()); return; }
        else
        {
            fsm->ChangeState(std::make_unique<Fall_State>()); return;
        }
    }


    // [ 마우스 방향 따라 FlipX ]
    fsm->GetPlayerFSM()->SetisBulletFliping(true);

    Vector2 playerPos = fsm->GetPlayerFSM()->GetTransform()->GetPosition();
    Vector2 mousePos = fsm->GetPlayerFSM()->MouseWorldPos;

    //if (mousePos.x < playerPos.x) fsm->GetPlayerFSM()->SetisBulletFlipX(true);  // 왼쪽
    //else fsm->GetPlayerFSM()->SetisBulletFlipX(false);

    if (mousePos.x < playerPos.x) fsm->GetPlayerFSM()->SetisBulletFlipX(false);  
    else fsm->GetPlayerFSM()->SetisBulletFlipX(true);



    // ----- [ 플레이어-마우스 위치 디버그 선 그리기 ] : 전체 거리 ----- 
    // 
    //D2D1_POINT_2F start = { playerPos.x, playerPos.y };     // 플레이어 월드 위치 
    //D2D1_POINT_2F end = { mousePos.x,  mousePos.y };        // 마우스 월드 위치 -> 실제 이동하는 거리 만큼 

    //// 월드 -> 스크린 변환용 
    //const auto& screenMatrix 
    //    = Transform::renderMatrix * Camera::GetMainInverseMatrix() * Transform::unityMatrix;

    //// 디버그 선 그리기
    //RenderSystem::Get().DebugDrawLine(start, end, screenMatrix, 2.0f);


    // ----- [ 플레이어-마우스 위치 디버그 선 그리기 ] : 실제 이동하는 거리 -----
    // 
    float baseMaxDist = fsm->GetPlayerFSM()->maxAttackDistance;
    float bonus = fsm->GetPlayerFSM()->GetAttackRangeBonus(); // 해금 레벨에 따른 거리 증가 
    float maxDistance = baseMaxDist * bonus;

    Vector2 toMouse = mousePos - playerPos;
    Vector2 direction = toMouse.Normalized();
    float actualDistance = ((toMouse.Magnitude()) < (maxDistance)) ? (toMouse.Magnitude()) : (maxDistance);

    Vector2 limitedTargetPos = playerPos + direction * actualDistance;

    D2D1_POINT_2F start = { playerPos.x, playerPos.y };
    D2D1_POINT_2F end = { limitedTargetPos.x, limitedTargetPos.y };

    // 월드 → 스크린 변환
    const auto& screenMatrix = Transform::renderMatrix * Camera::GetMainInverseMatrix() * Transform::unityMatrix;
    
    // 디버그 선 그리기
    RenderSystem::Get().DebugDrawLine(start, end, screenMatrix, 2.0f);

    // 잔상 
    afterimageTimer += unscaledDelta;
    if (afterimageTimer >= afterimageInterval)
    {
        afterimageTimer = 0.0f;

        CreateAfterImage(fsm);
    }

    if (GameManager::Get().isQuest && GameManager::Get().questIndex == 2)
        GameManager::Get().CheckQuest(2, 1);
}

void BulletTime_State::FixedUpdate(MovementFSM* fsm)
{

}

void BulletTime_State::Exit(MovementFSM* fsm)
{
    OutputDebugStringA("[BulletTime_State] (( 종료 - 불릿 타임 끝 )) \n");

    // 복구 
    Time::SetTimeScale(1.0f);
    fsm->GetPlayerFSM()->SetisBulletFliping(false);

    blackOut_renderer->SetEnabled(false);
}

void BulletTime_State::CreateAfterImage(MovementFSM* fsm)
{
    PlayerFSM* player = fsm->GetPlayerFSM();
    if (!player) return;

    // 현재 스프라이트 가져오기
    SpriteRenderer* playerRenderer = player->GetSpriteRenderer();
    if (!playerRenderer) return;

    shared_ptr<Sprite> currentSprite = playerRenderer->sprite;
    if (!currentSprite || !currentSprite->texture || !currentSprite->texture->texture2D)
    {
        OutputDebugStringA("AfterImage sprite에 texture가 없음!\n");
        return;
    }

    // 위치 및 방향
    Vector2 position = player->GetTransform()->GetPosition();
    bool flipX = playerRenderer->flipX;

    // 잔상 오브젝트 생성 
    GameObject* afterImage = player->Instantiate<GameObject>();
    afterImage->AddComponent<Transform>()->SetScale(player->GetTransform()->GetScale());
    afterImage->GetComponent<Transform>()->SetPosition(position);

    // 렌더러 추가
    auto renderer = afterImage->AddComponent<SpriteRenderer>();
    renderer->sprite = currentSprite;
    renderer->flipX = flipX;
    renderer->flipY = playerRenderer->flipY;
    renderer->SetAlpha(0.1f);
    renderer->SetColor(1.0f, 1.0f, 0.8f);
    renderer->renderMode = RenderMode::Lit_ColorTint;
    renderer->layer = 1;

    // 잔상 스크립트
    auto afterImageScript = afterImage->AddComponent<AfterImage>();
    afterImageScript->SetInitialAlpha(0.4f);
}