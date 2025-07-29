#include "Attack_State.h"

#include "ActionFSM.h" 
#include "PlayerFSM.h" 
#include "../Direct2D_EngineLib/Time.h"

void Attack_State::Enter(ActionFSM* fsm)
{
    OutputDebugStringA("[Attack_State] Player�� Attack_State ���� \n");

    // �ִϸ��̼� ��� 
    // fsm->GetPlayerFSM()->GetGameObject()->PlayAnimation("Attack1");
}

void Attack_State::Update(ActionFSM* fsm)  
{
    
    timer += Time::GetDeltaTime();

    // �ִϸ��̼� ������ Wait ���·� ��ȯ
    // [ Idle ] : �ӽ÷� ���� �ð� �� Wait ���·� ��ȯ
    if (timer > 1.0f)
    {
        fsm->GetPlayerFSM()->GetActionFSM()->ChangeState(std::make_unique<Wait_State>());
        return;
    }
}

void Attack_State::Exit(ActionFSM* fsm) {}