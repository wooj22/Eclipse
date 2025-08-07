#include "Hanging_State.h"
#include "Jump_Wall_State.h"
#include "Jump_State.h"
#include "Idle_State.h"
#include "Fall_State.h"

#include "MovementFSM.h"
#include "PlayerFSM.h"
#include "PlayerAnimatorController.h"
#include "GameManager.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"


void Hanging_State::Enter(MovementFSM * fsm)
{
    OutputDebugStringA("[Hanging_State] ���� �Ŵ޸� ���� ����\n");

    // ���� �ӵ��� 0���� (���� �Ŵ޸� ȿ��)
    fsm->GetPlayerFSM()->GetRigidbody()->velocity.y = 0.0f;
    fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = 0.0f;

    fsm->GetPlayerFSM()->SetLastFlipX(!fsm->GetPlayerFSM()->GetLastFlipX());

    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Hanging", true);
}

void Hanging_State::Update(MovementFSM* fsm)
{
    // ���� Ű ������ �� ������ ��ȯ
    if (GameManager::Get().CheckUnlock(SkillType::WallJump) && fsm->GetPlayerFSM()->GetIsSpace() )
    {
        fsm->ChangeState(std::make_unique<Jump_Wall_State>());
        return;
    }

    // ���� �������
    if (!fsm->GetPlayerFSM()->GetIsWallLeft() && !fsm->GetPlayerFSM()->GetIsWallRight())
    {
        fsm->ChangeState(std::make_unique<Idle_State>());
        return;
    }

    // �ݴ� ����Ű �Է��ϸ� ������ 
    float inputX = fsm->GetPlayerFSM()->GetInputX();
    if ((fsm->GetPlayerFSM()->GetIsWallLeft() && inputX > 0.5f) || (fsm->GetPlayerFSM()->GetIsWallRight() && inputX < -0.5f))
    {
        fsm->GetPlayerFSM()->SetLastFlipX(!fsm->GetPlayerFSM()->GetLastFlipX());

        fsm->ChangeState(std::make_unique<Fall_State>());
        return;
    }

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

    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Hanging", false);
}
