#include "Attack_State.h"
#include "Idle_State.h"
#include "MovementFSM.h" 
#include "PlayerFSM.h" 
#include "PlayerAnimatorController.h"

#include "../Direct2D_EngineLib/Time.h"

void Attack_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Attack_State] Player의 Attack_State 진입 \n");

    // 애니메이션 재생 
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Attack", true);
}

void Attack_State::Update(MovementFSM* fsm)
{
    timer += Time::GetDeltaTime();


    // [ Idle ] : 임시로 일정 시간 후 Idle 상태로 전환
    if (timer > 0.5f)
    {
        fsm->GetPlayerFSM()->GetAnimatorController()->PlayAnimation("Samurai_Idle"); // 임시 전환 
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