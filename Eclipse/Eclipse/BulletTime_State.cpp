#include "BulletTime_State.h"
#include "ActionFSM.h"
#include "PlayerFSM.h"
#include "../Direct2D_EngineLib/Time.h"
#include "Wait_State.h"

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

    // 공격 취소
    if (fsm->GetPlayerFSM()->GetIsRButton()) fsm->ChangeState(std::make_unique<Wait_State>());

    // 불릿 타임 끝 
    if (timer >= bulletTimeDuration)
    {
        Time::SetTimeScale(1.0f); // 시간 복구
        fsm->ChangeState(std::make_unique<Wait_State>());
    }
}

void BulletTime_State::Exit(ActionFSM* fsm)
{
    Time::SetTimeScale(1.0f); // 혹시 모르니 다시 복구
    OutputDebugStringA("[BulletTime_State] (( 종료 - 불릿 타임 끝 )) \n");
}