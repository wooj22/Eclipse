#include "Attack_State.h"

#include "ActionFSM.h" 
#include "PlayerFSM.h" 
#include "../Direct2D_EngineLib/Time.h"

void Attack_State::Enter(ActionFSM* fsm)
{
    OutputDebugStringA("[Attack_State] Player의 Attack_State 진입 \n");

    // 애니메이션 재생 
    // fsm->GetPlayerFSM()->GetGameObject()->PlayAnimation("Attack1");
}

void Attack_State::Update(ActionFSM* fsm)  
{
    
    timer += Time::GetDeltaTime();

    // 애니메이션 끝나면 Wait 상태로 전환
    // [ Idle ] : 임시로 일정 시간 후 Wait 상태로 전환
    if (timer > 1.0f)
    {
        fsm->GetPlayerFSM()->GetActionFSM()->ChangeState(std::make_unique<Wait_State>());
        return;
    }
}

void Attack_State::Exit(ActionFSM* fsm) {}