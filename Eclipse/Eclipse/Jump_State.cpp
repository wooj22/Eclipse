#include "Jump_State.h"
#include "Walk_State.h"
#include "Idle_State.h"
#include "Jump_Wall_State.h"

#include "MovementFSM.h" 
#include "PlayerFSM.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "Hanging_State.h"


void Jump_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Jump_State] Player의 Jump_State 진입 \n");

    // 초기화 
    canDoubleJump = true;
    timer = 0.0f;

    // 첫번째 Jump 실행 
    fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(0, fsm->GetPlayerFSM()->GetJumpForce()));
}

void Jump_State::Update(MovementFSM* fsm)
{
    timer += Time::GetDeltaTime();

    // [ Jump_Wall ]
    if (!fsm->GetPlayerFSM()->GetIsGround() && fsm->GetPlayerFSM()->GetIsSpace())
    {
        if (fsm->GetPlayerFSM()->GetIsWallLeft() || fsm->GetPlayerFSM()->GetIsWallRight())
        {
            fsm->ChangeState(std::make_unique<Jump_Wall_State>());
            return;
        }
    }

    // 두번째 Jump 실행 
    if (!fsm->GetPlayerFSM()->GetIsGround() && fsm->GetPlayerFSM()->GetIsSpace() && canDoubleJump)
    {
        fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(0, fsm->GetPlayerFSM()->GetJumpForce()));
        canDoubleJump = false;
    }

    // [ Hanging ]
    if (!fsm->GetPlayerFSM()->GetIsGround())
    {
        if (fsm->GetPlayerFSM()->GetIsWallLeft() && fsm->GetPlayerFSM()->GetInputX() < -0.5f)
        {
            fsm->ChangeState(std::make_unique<Hanging_State>());
            return;
        }
        else if (fsm->GetPlayerFSM()->GetIsWallRight() && fsm->GetPlayerFSM()->GetInputX() > 0.5f)
        {
            fsm->ChangeState(std::make_unique<Hanging_State>());
            return;
        }
    }

    // [ Idle ] : 일정 시간 후에만 감지
    if (fsm->GetPlayerFSM()->GetIsGround() && timer > coyoteTime)
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Idle_State>());
        return;
    }
}

void Jump_State::FixedUpdate(MovementFSM* fsm)
{
    // 매 FixedUpdate마다 최신 입력 갱신
    inputX = fsm->GetPlayerFSM()->GetInputX();
    curVelX = fsm->GetPlayerFSM()->GetRigidbody()->velocity.x;

    // 입력이 있는 경우: 목표 속도로 보간
    if (inputX != 0.0f)
    {
        float targetVelX = inputX * fsm->GetPlayerFSM()->GetCurSpeed();
        fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = Lerp(curVelX, targetVelX, Time::GetDeltaTime() * airAcceleration);
    }
    else
    {
        // 입력이 없으면 감속
        fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = Lerp(curVelX, 0.0f, Time::GetDeltaTime() * airFriction);
    }
}

void Jump_State::Exit(MovementFSM* fsm)
{
    if (fsm->GetPlayerFSM()->GetIsGround())
    {
        canDoubleJump = true;  // 착지 시 더블 점프를 할 수 있도록 설정
    }
}