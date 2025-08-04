#include "BulletTime_State.h"
#include "Attack_State.h"
#include "MovementFSM.h"
#include "PlayerFSM.h"
#include "PlayerAnimatorController.h"

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
}

void BulletTime_State::Update(MovementFSM* fsm)
{
    float delta = Time::GetDeltaTime();
    float unscaledDelta = delta / Time::GetTimeScale();  // timeScale 나눠서 실제 시간 계산

    fsm->GetPlayerFSM()->timer += unscaledDelta;

    // [ Attack ] 마우스 왼쪽 버튼에서 손을 뗐을 때 → 공격
    if (Input::GetKeyUp(VK_LBUTTON))
    {
        Time::SetTimeScale(1.0f); 
        fsm->ChangeState(std::make_unique<Attack_State>());
        return;
    }

    // [ Idle ] 공격 취소 
    if (fsm->GetPlayerFSM()->GetIsRButton()) 
    { 
        if (fsm->GetPlayerFSM()->GetIsGround()) fsm->ChangeState(std::make_unique<Idle_State>());
        else
        {
            // Fall 상태
        }
    }

    // [ Idle ] 불릿 타임 끝 
    if (fsm->GetPlayerFSM()->timer >= fsm->GetPlayerFSM()->bulletTimeDuration)
    {
        Time::SetTimeScale(1.0f); // 시간 복구

        if (fsm->GetPlayerFSM()->GetIsGround()) fsm->ChangeState(std::make_unique<Idle_State>());
        else
        {
            // Fall 상태
        }
    }


    // [ 마우스 방향 따라 FlipX ]
    fsm->GetPlayerFSM()->SetisBulletFliping(true);

    Vector2 playerPos = fsm->GetPlayerFSM()->GetTransform()->GetPosition();
    Vector2 mousePos = fsm->GetPlayerFSM()->MouseWorldPos;

    if (mousePos.x < playerPos.x) fsm->GetPlayerFSM()->SetisBulletFlipX(true);  // 왼쪽
    else fsm->GetPlayerFSM()->SetisBulletFlipX(false);



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
    float maxDistance = baseMaxDist + bonus;

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
}

void BulletTime_State::FixedUpdate(MovementFSM* fsm)
{

}

void BulletTime_State::Exit(MovementFSM* fsm)
{
    OutputDebugStringA("[BulletTime_State] (( 종료 - 불릿 타임 끝 )) \n");

    // 복구 
    fsm->GetPlayerFSM()->SetisBulletFliping(false);
    Time::SetTimeScale(1.0f); 
}