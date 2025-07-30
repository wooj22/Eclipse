#include "Jump_State.h"
#include "Walk_State.h"
#include "Idle_State.h"
#include "MovementFSM.h" 
#include "PlayerFSM.h"
#include "PlayerAnimatorController.h"

#include "../Direct2D_EngineLib/Rigidbody.h"

void Walk_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Walk_State] Player의 Walk_State 진입 \n");

    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Walk", true);
}

void Walk_State::Update(MovementFSM* fsm)
{
    // [ Jump ]
    if (fsm->GetPlayerFSM()->GetIsSpace() && fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Jump_State>());
    }
}

void Walk_State::FixedUpdate(MovementFSM* fsm)
{
    // Move 실행 
    fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = fsm->GetPlayerFSM()->GetInputX() * fsm->GetPlayerFSM()->GetCurSpeed();

    // [ Idle ]
    if (!(fsm->GetPlayerFSM()->GetIsA() || fsm->GetPlayerFSM()->GetIsD()))
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Idle_State>());
    }
}

void Walk_State::Exit(MovementFSM* fsm)
{

}