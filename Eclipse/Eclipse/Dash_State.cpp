#include "Dash_State.h"
#include "Idle_State.h"
#include "MovementFSM.h"
#include "PlayerFSM.h"
#include "Fall_State.h"
#include "PlayerAnimatorController.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Time.h"
#include "AfterImage.h"


void Dash_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Dash_State] Dash 시작\n");

    dashTimer = 0.0f;

    // 무적 처리
    fsm->GetPlayerFSM()->SetInvincible(true);

    // 물리 초기화 
    fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2(0, 0);
    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = false;
    
    // 대시 
    moveBonus = fsm->GetPlayerFSM()->GetMoveSpeedBonus();
    dashSpeed = (fsm->GetPlayerFSM()->GetWalkSpeed() + moveBonus) * 1.5f; // 150%

    inputX = fsm->GetPlayerFSM()->GetInputX();

    // 입력이 없을 경우 바라보는 방향으로 설정
    if (inputX == 0)
    {
        bool flipX = fsm->GetPlayerFSM()->GetLastFlipX();
        // inputX = flipX ? -1 : 1;  // true: 왼쪽 → -1, false: 오른쪽 → +1
        inputX = flipX ? 1 : -1;  
    }

    // 현재 위치 기준으로 위로 riseAmount 만큼 띄움
    Vector2 currentPos = fsm->GetPlayerFSM()->GetTransform()->GetPosition();
    targetYPosition = currentPos.y - riseAmount; // Y축 음수 방향이 위라면 -riseAmount

    Vector2 newPos = Vector2(currentPos.x, targetYPosition);
    fsm->GetPlayerFSM()->GetTransform()->SetPosition(newPos);


    // 애니메이션
    if (!fsm->GetPlayerFSM()->isReleaseSkillAvailable) fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Dash", true);
    else fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Y_Player_Dash", true);

}

void Dash_State::Update(MovementFSM* fsm)
{
    dashTimer += Time::GetDeltaTime();

    if (dashTimer >= dashDuration)
    {
        // 무적 해제
        fsm->GetPlayerFSM()->SetInvincible(false);  

        // 상태 전환
        if (fsm->GetPlayerFSM()->GetIsGround()) fsm->ChangeState(std::make_unique<Idle_State>());
        else fsm->ChangeState(std::make_unique<Fall_State>());

        return;
    }
}

void Dash_State::FixedUpdate(MovementFSM* fsm)
{
    // X축 속도만 적용 (Y는 고정)
    fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2(inputX * dashSpeed, 0.0f);

    // Y위치 고정
    Vector2 currentPos = fsm->GetPlayerFSM()->GetTransform()->GetPosition();
    currentPos.y = targetYPosition;
    fsm->GetPlayerFSM()->GetTransform()->SetPosition(currentPos);

    //std::string debugStr = "[Dash_State] velocity.y = " + std::to_string(fsm->GetPlayerFSM()->GetRigidbody()->velocity.y) + "\n";
    //OutputDebugStringA(debugStr.c_str());

    afterimageTimer += Time::GetDeltaTime();
    if (afterimageTimer >= afterimageInterval)
    {
        afterimageTimer = 0.0f;

        CreateAfterImage(fsm);
    }
}

void Dash_State::Exit(MovementFSM* fsm)
{
    OutputDebugStringA("[Dash_State] Dash 종료\n");

    // 물리 초기화 
    fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2(0, 0);
    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = true;

    // 대시 후 쿨타임 설정
    fsm->GetPlayerFSM()->ResetDashCooldown(); 

    // 애니메이션 종료
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Dash", false);
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Y_Player_Dash", false);

    //if (!fsm->GetPlayerFSM()->isReleaseSkillAvailable) fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Dash", false);
    //else fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Y_Player_Dash", false);
}


void Dash_State::CreateAfterImage(MovementFSM* fsm)
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
