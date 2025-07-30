#include "Hanging_State.h"
#include "Jump_Wall_State.h"
#include "Jump_State.h"
#include "Idle_State.h"

#include "MovementFSM.h"
#include "PlayerFSM.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"

void Hanging_State::Enter(MovementFSM * fsm)
{
    OutputDebugStringA("[Hanging_State] ���� �Ŵ޸� ���� ����\n");

    // ���� �ӵ��� 0���� (���� �Ŵ޸� ȿ��)
    fsm->GetPlayerFSM()->GetRigidbody()->velocity.y = 0.0f;
}

void Hanging_State::Update(MovementFSM* fsm)
{
    PlayerFSM* player = fsm->GetPlayerFSM();

    // 1. ���� Ű ������ �� ������ ��ȯ
    if (player->GetIsSpace())
    {
        fsm->ChangeState(std::make_unique<Jump_Wall_State>());
        return;
    }

    // 2. ���� ������ų� ����Ű�� ���� �ٽ� ���� ����
    if (!player->GetIsWallLeft() && !player->GetIsWallRight())
    {
        fsm->ChangeState(std::make_unique<Idle_State>());
        return;
    }

    //float inputX = player->GetInputX();
    //if ((player->GetIsWallLeft() && inputX > -0.5f) ||(player->GetIsWallRight() && inputX < 0.5f))
    //{
    //    fsm->ChangeState(std::make_unique<Jump_State>());
    //    return;
    //}

    // [ Idle ] 
    if (fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Idle_State>());
        return;
    }
}

void Hanging_State::FixedUpdate(MovementFSM* fsm)
{
    // �߷��� ���ϰ� ���� (õõ�� ��������)
    fsm->GetPlayerFSM()->GetRigidbody()->gravityScale = 10;
}

void Hanging_State::Exit(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->GetRigidbody()->gravityScale = 80;
}
