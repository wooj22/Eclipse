#include "Fall_State.h"
#include "Idle_State.h"
#include "Walk_State.h"
#include "Jump_State.h"
#include "Attack_State.h"
#include "BulletTime_State.h"
#include "Hanging_State.h"
#include "Dash_State.h"

#include "MovementFSM.h" 
#include "PlayerFSM.h"
#include "Player.h"
#include "PlayerAnimatorController.h"
#include "GameManager.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"



void Fall_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Fall_State] Player�� Fall_State ���� \n");

    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = true;

    // �ִϸ��̼� ���
    if (!fsm->GetPlayerFSM()->isAbsorbSkillActive) fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Jump", true);
    else fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Y_Player_Jump", true);

}

void Fall_State::Update(MovementFSM* fsm)
{
    // [ Idle ]
    if (fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Idle_State>());
        return;
    }

    // [ Jump ] �ر� ���� Ȯ��
    if (GameManager::Get().CheckUnlock(SkillType::JumpAttackExtra) &&
        fsm->GetPlayerFSM()->canDoubleJump &&
        !fsm->GetPlayerFSM()->GetIsGround() &&
        fsm->GetPlayerFSM()->GetIsSpace())
    {
        fsm->GetPlayerFSM()->canDoubleJump = false;  // ��� ó��
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Jump_State>());
        return;
    }

    // [ Hanging ]
    if (!fsm->GetPlayerFSM()->GetIsGround() && GameManager::Get().CheckUnlock(SkillType::WallJump) && fsm->GetPlayerFSM()->canHanging)
    {
        if (fsm->GetPlayerFSM()->GetIsWallLeft() && fsm->GetPlayerFSM()->GetInputX() < -0.5f)
        {
            fsm->GetPlayerFSM()->canHanging = false;
            fsm->ChangeState(std::make_unique<Hanging_State>());
            return;
        }
        else if (fsm->GetPlayerFSM()->GetIsWallRight() && fsm->GetPlayerFSM()->GetInputX() > 0.5f)
        {
            fsm->GetPlayerFSM()->canHanging = false;
            fsm->ChangeState(std::make_unique<Hanging_State>());
            return;
        }
    }

    // [ Dash ]
    if (fsm->GetPlayerFSM()->GetisShift() && GameManager::Get().CheckUnlock(SkillType::Dash) && fsm->GetPlayerFSM()->CanDash())
    {
        fsm->ChangeState(std::make_unique<Dash_State>());
        return;
    }
}

void Fall_State::FixedUpdate(MovementFSM* fsm) 
{
    // ���� �ϰ� 
    if (GameManager::Get().CheckUnlock(SkillType::FastFall) && fsm->GetPlayerFSM()->GetIsS())
    {
        fsm->GetPlayerFSM()->GetRigidbody()->gravityScale = fsm->GetPlayerFSM()->fastFallGravity;
    }

    // [ �¿� �̵� ]
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

void Fall_State::Exit(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->GetRigidbody()->gravityScale = fsm->GetPlayerFSM()->defaultGravity;

    if (!fsm->GetPlayerFSM()->isAbsorbSkillActive) fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Jump", false);
    else fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Y_Player_Jump", false);
}