#include "BulletTime_State.h"
#include "ActionFSM.h"
#include "PlayerFSM.h"
#include "../Direct2D_EngineLib/Time.h"
#include "Wait_State.h"
#include "../Direct2D_EngineLib/Input.h"
#include "Attack_State.h"

void BulletTime_State::Enter(ActionFSM* fsm)
{
    OutputDebugStringA("[BulletTime_State] (( 진입 - 불릿 타임 시작 )) \n");
    Time::SetTimeScale(0.1f); // 시간 느리게
    timer = 0.0f;
}

void BulletTime_State::Update(ActionFSM* fsm)
{
    float delta = Time::GetDeltaTime();
    float unscaledDelta = delta / Time::GetTimeScale();  // timeScale 나눠서 실제 시간 계산

    timer += unscaledDelta;

    // [ wait ] 공격 취소 
    if (fsm->GetPlayerFSM()->GetIsRButton()) fsm->ChangeState(std::make_unique<Wait_State>());

    // [ Attack ] 마우스 왼쪽 버튼에서 손을 뗐을 때 → 공격
    if (Input::GetKeyUp(VK_LBUTTON))
    {
        Time::SetTimeScale(1.0f); 
        fsm->ChangeState(std::make_unique<Attack_State>());
        return;
    }

    // 불릿 타임 끝 
    if (timer >= bulletTimeDuration)
    {
        Time::SetTimeScale(1.0f); // 시간 복구
        fsm->ChangeState(std::make_unique<Wait_State>());
    }

    // 마우스 위치 따라서 플레이어가 바라보도록 하기 
}

void BulletTime_State::Exit(ActionFSM* fsm)
{
    Time::SetTimeScale(1.0f); // 혹시 모르니 다시 복구
    OutputDebugStringA("[BulletTime_State] (( 종료 - 불릿 타임 끝 )) \n");
}