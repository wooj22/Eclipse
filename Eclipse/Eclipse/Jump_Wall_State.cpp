#include "Jump_Wall_State.h"
#include "Idle_State.h"
#include "MovementFSM.h"
#include "PlayerFSM.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"

void Jump_Wall_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Jump_Wall_State] 벽 점프 상태 진입\n");

    wallJumpForce = fsm->GetPlayerFSM()->GetJumpForce();

    Vector2 force;

    if (fsm->GetPlayerFSM()->GetIsWallLeft())
    {
        fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(+jumpXPower, wallJumpForce));  // 오른쪽 방향으로 튕김
    }
    else if (fsm->GetPlayerFSM()->GetIsWallRight())
    {
        fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(-jumpXPower, wallJumpForce));  // 왼쪽 방향으로 튕김
    }

    fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2(0, 0); // 수직 속도 초기화
}

void Jump_Wall_State::Update(MovementFSM* fsm)
{
    if (fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->ChangeState(std::make_unique<Idle_State>());
        return;
    }
}

void Jump_Wall_State::FixedUpdate(MovementFSM* fsm)
{
    inputX = fsm->GetPlayerFSM()->GetInputX();
    curVelX = fsm->GetPlayerFSM()->GetRigidbody()->velocity.x;

    // 입력이 있는 경우: 목표 속도로 보간
    if (inputX != 0.0f)
    {
        curVelX = inputX * fsm->GetPlayerFSM()->GetCurSpeed();
        fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = Lerp(curVelX, curVelX, Time::GetDeltaTime() * airAcceleration);
    }
    else
    {
        // 입력 없으면 감속
        fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = Lerp(curVelX, 0.0f, Time::GetDeltaTime() * airFriction);
    }
}

void Jump_Wall_State::Exit(MovementFSM* fsm)
{
}