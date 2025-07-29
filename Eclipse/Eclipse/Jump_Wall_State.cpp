#include "Jump_Wall_State.h"
#include "Idle_State.h"
#include "MovementFSM.h"
#include "PlayerFSM.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "Hanging_State.h"

void Jump_Wall_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Jump_Wall_State] �� ���� ���� ����\n");

    elapsedTime = 0.0f;  // �ð� �ʱ�ȭ
    wallJumpForce = fsm->GetPlayerFSM()->GetJumpForce();
    fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2(0, 0); // ���� �ӵ� �ʱ�ȭ

    if (fsm->GetPlayerFSM()->GetIsWallLeft())
    {
        fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(+jumpXPower, wallJumpForce));  // ������ �������� ƨ��
        lastWallDir = -1;
    }
    else if (fsm->GetPlayerFSM()->GetIsWallRight())
    {
        fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(-jumpXPower, wallJumpForce));  // ���� �������� ƨ��
        lastWallDir = 1;
    }
    else
    {
        lastWallDir = 0;
    }

    // ������ ���
    // fsm->GetPlayerFSM()->SetLastWallDir(lastWallDir);
}

void Jump_Wall_State::Update(MovementFSM* fsm)
{
    if (fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->ChangeState(std::make_unique<Idle_State>());
        return;
    }

    // [ Hanging ] 
    if (elapsedTime < hangingBlockTime) return;

    if (fsm->GetPlayerFSM()->GetIsWallLeft() && fsm->GetPlayerFSM()->GetInputX() < -0.5f)
    {
        fsm->ChangeState(std::make_unique<Hanging_State>());
        return;
    }
    else if (fsm->GetPlayerFSM()->GetIsWallRight() && fsm->GetPlayerFSM()->GetInputX() > 0.5f)
    {
        fsm->ChangeState(std::make_unique<Hanging_State>());
        return;
    }
}

void Jump_Wall_State::FixedUpdate(MovementFSM* fsm)
{
    elapsedTime += Time::GetDeltaTime();

    if (elapsedTime < inputBlockTime) return;

    inputX = fsm->GetPlayerFSM()->GetInputX();
    curVelX = fsm->GetPlayerFSM()->GetRigidbody()->velocity.x;

    // �Է��� �ִ� ���: ��ǥ �ӵ��� ����
    if (inputX != 0.0f)
    {
        curVelX = inputX * fsm->GetPlayerFSM()->GetCurSpeed();
        fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = Lerp(curVelX, curVelX, Time::GetDeltaTime() * airAcceleration);
    }
    else
    {
        // �Է� ������ ����
        fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = Lerp(curVelX, 0.0f, Time::GetDeltaTime() * airFriction);
    }
}

void Jump_Wall_State::Exit(MovementFSM* fsm)
{
}