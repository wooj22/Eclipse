#include "Idle_State.h"
#include "Walk_State.h"
#include "Jump_State.h"
#include "MovementFSM.h" 
#include "PlayerFSM.h"
#include "PlayerAnimatorController.h"

#include "../Direct2D_EngineLib/Rigidbody.h"

void Idle_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Idle_State] Player�� Idle_State ���� \n");

    fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = 0.0f;         // �������� �־��ٸ� ���� 

    // �ִϸ��̼� ���
    // fsm->GetPlayerFSM()->GetAnimatorController()->PlayAnimation("Samurai_Idle");
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Idle", true);
}

void Idle_State::Update(MovementFSM* fsm)
{
    // [ Jump ]
    if (fsm->GetPlayerFSM()->GetIsSpace() && fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Jump_State>());
    }

    // [ Walk ]
    if (fsm->GetPlayerFSM()->GetIsA() || fsm->GetPlayerFSM()->GetIsD())
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Walk_State>());
    }
}

void Idle_State::FixedUpdate(MovementFSM* fsm)
{

}

void Idle_State::Exit(MovementFSM* fsm)
{

}