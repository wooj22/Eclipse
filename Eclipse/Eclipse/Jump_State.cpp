#include "Jump_State.h"
#include "Walk_State.h"
#include "Idle_State.h"
#include "Jump_Wall_State.h"

#include "MovementFSM.h" 
#include "PlayerFSM.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"


void Jump_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Jump_State] Player의 Jump_State 진입 \n");

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

    // [ Idle ] : 일정 시간 후에만 감지
    if (fsm->GetPlayerFSM()->GetIsGround() && timer > coyoteTime)
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Idle_State>());
        return;
    }
}

void Jump_State::FixedUpdate(MovementFSM* fsm)
{

}

void Jump_State::Exit(MovementFSM* fsm)
{
    if (fsm->GetPlayerFSM()->GetIsGround())
    {
        canDoubleJump = true;  // 착지 시 더블 점프를 할 수 있도록 설정
    }
}