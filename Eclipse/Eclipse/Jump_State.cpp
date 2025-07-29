#include "Jump_State.h"
#include "Walk_State.h"
#include "Idle_State.h"
#include "Jump_Wall_State.h"

#include "MovementFSM.h" 
#include "PlayerFSM.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "Hanging_State.h"


void Jump_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Jump_State] Player�� Jump_State ���� \n");

    // �ʱ�ȭ 
    canDoubleJump = true;
    timer = 0.0f;

    // ù��° Jump ���� 
    fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(0, fsm->GetPlayerFSM()->GetJumpForce()));
}

void Jump_State::Update(MovementFSM* fsm)
{
    timer += Time::GetDeltaTime();

    // [ Jump_Wall ]
    if (!fsm->GetPlayerFSM()->GetIsGround() && fsm->GetPlayerFSM()->GetIsSpace())
    {
        if (fsm->GetPlayerFSM()->GetIsWallLeft() || fsm->GetPlayerFSM()->GetIsWallRight())
        {
            fsm->ChangeState(std::make_unique<Jump_Wall_State>());
            return;
        }
    }

    // �ι�° Jump ���� 
    if (!fsm->GetPlayerFSM()->GetIsGround() && fsm->GetPlayerFSM()->GetIsSpace() && canDoubleJump)
    {
        fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(0, fsm->GetPlayerFSM()->GetJumpForce()));
        canDoubleJump = false;
    }

    // [ Hanging ]
    if (!fsm->GetPlayerFSM()->GetIsGround())
    {
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

    // [ Idle ] : ���� �ð� �Ŀ��� ����
    if (fsm->GetPlayerFSM()->GetIsGround() && timer > coyoteTime)
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Idle_State>());
        return;
    }
}

void Jump_State::FixedUpdate(MovementFSM* fsm)
{
    // �� FixedUpdate���� �ֽ� �Է� ����
    inputX = fsm->GetPlayerFSM()->GetInputX();
    curVelX = fsm->GetPlayerFSM()->GetRigidbody()->velocity.x;

    // �Է��� �ִ� ���: ��ǥ �ӵ��� ����
    if (inputX != 0.0f)
    {
        float targetVelX = inputX * fsm->GetPlayerFSM()->GetCurSpeed();
        fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = Lerp(curVelX, targetVelX, Time::GetDeltaTime() * airAcceleration);
    }
    else
    {
        // �Է��� ������ ����
        fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = Lerp(curVelX, 0.0f, Time::GetDeltaTime() * airFriction);
    }
}

void Jump_State::Exit(MovementFSM* fsm)
{
    if (fsm->GetPlayerFSM()->GetIsGround())
    {
        canDoubleJump = true;  // ���� �� ���� ������ �� �� �ֵ��� ����
    }
}