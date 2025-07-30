#include "Wait_State.h"
#include "PlayerFSM.h" 
#include "ActionFSM.h" 
#include "Attack_State.h"

void Wait_State::Enter(ActionFSM* fsm)
{
    OutputDebugStringA("[Wait_State] PlayerÀÇ Wait_State ÁøÀÔ \n");

    // fsm->GetPlayerFSM()->GetGameObject()->PlayAnimation("Attack1");
}

void Wait_State::Update(ActionFSM* fsm)
{
    // [ Attack ]
    if (fsm->GetPlayerFSM()->GetIsLButton()/* && fsm->GetPlayerFSM()->GetIsGround()*/)
    {
        fsm->GetPlayerFSM()->GetActionFSM()->ChangeState(std::make_unique<Attack_State>());
    }
}

void Wait_State::Exit(ActionFSM* fsm) {}