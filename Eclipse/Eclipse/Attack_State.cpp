#include "Attack_State.h"
#include "Idle_State.h"
#include "MovementFSM.h" 
#include "PlayerFSM.h" 
#include "PlayerAnimatorController.h"

#include "../Direct2D_EngineLib/Time.h"

void Attack_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Attack_State] Player�� Attack_State ���� \n");

    // �ִϸ��̼� ��� 
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Attack", true);
}

void Attack_State::Update(MovementFSM* fsm)
{
    timer += Time::GetDeltaTime();


    // [ Idle ] : �ӽ÷� ���� �ð� �� Idle ���·� ��ȯ
    if (timer > 0.5f)
    {
        fsm->GetPlayerFSM()->GetAnimatorController()->PlayAnimation("Samurai_Idle"); // �ӽ� ��ȯ 
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Idle_State>());
        return;
    }

    // [ Idle ] 

}

void Attack_State::FixedUpdate(MovementFSM* fsm)
{

}

void Attack_State::Exit(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Attack", false);
}