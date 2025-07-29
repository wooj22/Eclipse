#include "BulletTime_State.h"
#include "ActionFSM.h"
#include "PlayerFSM.h"
#include "../Direct2D_EngineLib/Time.h"
#include "Wait_State.h"
#include "../Direct2D_EngineLib/Input.h"
#include "Attack_State.h"

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

    // [ wait ] ���� ��� 
    if (fsm->GetPlayerFSM()->GetIsRButton()) fsm->ChangeState(std::make_unique<Wait_State>());

    // [ Attack ] ���콺 ���� ��ư���� ���� ���� �� �� ����
    if (Input::GetKeyUp(VK_LBUTTON))
    {
        Time::SetTimeScale(1.0f); 
        fsm->ChangeState(std::make_unique<Attack_State>());
        return;
    }

    // �Ҹ� Ÿ�� �� 
    if (timer >= bulletTimeDuration)
    {
        Time::SetTimeScale(1.0f); // �ð� ����
        fsm->ChangeState(std::make_unique<Wait_State>());
    }

    // ���콺 ��ġ ���� �÷��̾ �ٶ󺸵��� �ϱ� 
}

void BulletTime_State::Exit(ActionFSM* fsm)
{
    Time::SetTimeScale(1.0f); // Ȥ�� �𸣴� �ٽ� ����
    OutputDebugStringA("[BulletTime_State] (( ���� - �Ҹ� Ÿ�� �� )) \n");
}