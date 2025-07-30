#include "Jump_Wall_State.h"
#include "Idle_State.h"
#include "MovementFSM.h"
#include "PlayerFSM.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"

void Jump_Wall_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Jump_Wall_State] �� ���� ���� ����\n");

    Rigidbody* rb = fsm->GetPlayerFSM()->GetRigidbody();
    Vector2 force;

    if (fsm->GetPlayerFSM()->GetIsWallLeft())
    {
        force = Vector2(+jumpXPower, wallJumpForce);  // ������ �������� ƨ��
    }
    else if (fsm->GetPlayerFSM()->GetIsWallRight())
    {
        force = Vector2(-jumpXPower, wallJumpForce);  // ���� �������� ƨ��
    }

    rb->velocity = Vector2(0, 0); // ���� �ӵ� �ʱ�ȭ
    rb->AddImpulse(force);
}

void Jump_Wall_State::Update(MovementFSM* fsm)
{
    if (fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->ChangeState(std::make_unique<Idle_State>());
        return;
    }
}

void Jump_Wall_State::FixedUpdate(MovementFSM* fsm)
{

}

void Jump_Wall_State::Exit(MovementFSM* fsm)
{
}