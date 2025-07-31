#include "BulletTime_State.h"
#include "Attack_State.h"
#include "MovementFSM.h"
#include "PlayerFSM.h"
#include "PlayerAnimatorController.h"

#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/RenderSystem.h"
#include "../Direct2D_EngineLib/Vector2.h"
#include "../Direct2D_EngineLib/Camera.h"

void BulletTime_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[BulletTime_State] (( ���� - �Ҹ� Ÿ�� ���� )) \n");
    Time::SetTimeScale(0.3f); // �ð� ������

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


    // [ ���콺 ���� ���� FlipX ]
    fsm->GetPlayerFSM()->SetisBulletFliping(true);

    Vector2 playerPos = fsm->GetPlayerFSM()->GetTransform()->GetPosition();
    Vector2 mousePos = fsm->GetPlayerFSM()->MouseWorldPos;

    if (mousePos.x < playerPos.x) fsm->GetPlayerFSM()->SetisBulletFlipX(true);  // ����
    else fsm->GetPlayerFSM()->SetisBulletFlipX(false);



    // [ �÷��̾�-���콺 ��ġ ����� �� �׸��� ]

    D2D1_POINT_2F start = { playerPos.x, playerPos.y };     // �÷��̾� ���� ��ġ 
    D2D1_POINT_2F end = { mousePos.x,  mousePos.y };        // ���콺 ���� ��ġ 

    // ���� -> ��ũ�� ��ȯ�� 
    const auto& screenMatrix 
        = Transform::renderMatrix * Camera::GetMainInverseMatrix() * Transform::unityMatrix;

    // ����� �� �׸���
    RenderSystem::Get().DebugDrawLine(start, end, screenMatrix, 2.0f);
}

void BulletTime_State::FixedUpdate(MovementFSM* fsm)
{

}

void BulletTime_State::Exit(MovementFSM* fsm)
{
    OutputDebugStringA("[BulletTime_State] (( ���� - �Ҹ� Ÿ�� �� )) \n");

    fsm->GetPlayerFSM()->SetisBulletFliping(false);
    // fsm->GetPlayerFSM()->SetLastFlipX(false);

    Time::SetTimeScale(1.0f); // Ȥ�� �𸣴� �ٽ� ����
}