#include "BulletTime_State.h"
#include "Attack_State.h"
#include "MovementFSM.h"
#include "PlayerFSM.h"
#include "PlayerAnimatorController.h"

#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"

void BulletTime_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[BulletTime_State] (( ���� - �Ҹ� Ÿ�� ���� )) \n");
    Time::SetTimeScale(0.1f); // �ð� ������

    fsm->GetPlayerFSM()->timer = 0.0f;
}

void BulletTime_State::Update(MovementFSM* fsm)
{
    float delta = Time::GetDeltaTime();
    float unscaledDelta = delta / Time::GetTimeScale();  // timeScale ������ ���� �ð� ���

    fsm->GetPlayerFSM()->timer += unscaledDelta;

    // [ Attack ] ���콺 ���� ��ư���� ���� ���� �� �� ����
    if (Input::GetKeyUp(VK_LBUTTON))
    {
        Time::SetTimeScale(1.0f); 
        fsm->ChangeState(std::make_unique<Attack_State>());
        return;
    }

    // [ Idle ] ���� ��� 
    if (fsm->GetPlayerFSM()->GetIsRButton()) 
    { 
        if (fsm->GetPlayerFSM()->GetIsGround()) fsm->ChangeState(std::make_unique<Idle_State>());
        else
        {
            // Fall ����
        }
    }


    // [ Idle ] �Ҹ� Ÿ�� �� 
    if (fsm->GetPlayerFSM()->timer >= fsm->GetPlayerFSM()->bulletTimeDuration)
    {
        Time::SetTimeScale(1.0f); // �ð� ����

        if (fsm->GetPlayerFSM()->GetIsGround()) fsm->ChangeState(std::make_unique<Idle_State>());
        else
        {
            // Fall ����
        }
    }

    // ���콺 ��ġ ���� �÷��̾ �ٶ󺸵��� �ϱ� 


}

void BulletTime_State::FixedUpdate(MovementFSM* fsm)
{

}

void BulletTime_State::Exit(MovementFSM* fsm)
{
    OutputDebugStringA("[BulletTime_State] (( ���� - �Ҹ� Ÿ�� �� )) \n");
    Time::SetTimeScale(1.0f); // Ȥ�� �𸣴� �ٽ� ����
}