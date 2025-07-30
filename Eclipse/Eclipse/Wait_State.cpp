#include "Wait_State.h"
#include "PlayerFSM.h"
#include "ActionFSM.h"
#include "Attack_State.h"
#include "BulletTime_State.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"

void Wait_State::Enter(ActionFSM* fsm)
{
    OutputDebugStringA("[Wait_State] Player의 Wait_State 진입 \n");
    holdTime = 0.0f;
    isHolding = false;
    timer = 0.0f;
}

void Wait_State::Update(ActionFSM* fsm)
{
    timer += Time::GetDeltaTime();

    if (timer < ignoreInputDuration) return;


    if (Input::GetKey(VK_LBUTTON))
    {
        if (!isHolding)
        {
            isHolding = true;
            holdTime = 0.0f;
        }

        holdTime += Time::GetDeltaTime();

        // [ BulletTime ]
        if (holdTime >= bulletTimeThreshold)
        {
            // fsm->GetPlayerFSM()->GetActionFSM()->ChangeState(std::make_unique<BulletTime_State>());
        }
    }
    else
    {
        // [ Attack ]
        if (isHolding && holdTime < bulletTimeThreshold)
        {
            // fsm->GetPlayerFSM()->GetActionFSM()->ChangeState(std::make_unique<Attack_State>());
        }

        // 초기화
        isHolding = false;
        holdTime = 0.0f;
    }
}

void Wait_State::Exit(ActionFSM* fsm)
{
}
