#include "BulletTime_State.h"
#include "ActionFSM.h"
#include "PlayerFSM.h"
#include "../Direct2D_EngineLib/Time.h"
#include "Wait_State.h"

void BulletTime_State::Enter(ActionFSM* fsm)
{
    OutputDebugStringA("[BulletTime_State] (( ���� - �Ҹ� Ÿ�� ���� )) \n");
    Time::SetTimeScale(0.1f); // �ð� ������
    timer = 0.0f;
}

void BulletTime_State::Update(ActionFSM* fsm)
{
    float delta = Time::GetDeltaTime();
    float unscaledDelta = delta / Time::GetTimeScale();  // timeScale ������ ���� �ð� ���

    timer += unscaledDelta;

    // ���� ���
    if (fsm->GetPlayerFSM()->GetIsRButton()) fsm->ChangeState(std::make_unique<Wait_State>());

    // �Ҹ� Ÿ�� �� 
    if (timer >= bulletTimeDuration)
    {
        Time::SetTimeScale(1.0f); // �ð� ����
        fsm->ChangeState(std::make_unique<Wait_State>());
    }
}

void BulletTime_State::Exit(ActionFSM* fsm)
{
    Time::SetTimeScale(1.0f); // Ȥ�� �𸣴� �ٽ� ����
    OutputDebugStringA("[BulletTime_State] (( ���� - �Ҹ� Ÿ�� �� )) \n");
}