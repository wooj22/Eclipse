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
    fsm->GetPlayerFSM()->timer = 0.0f;
    fsm->GetPlayerFSM()->didFastFall = false; 

    // �ִϸ��̼� ���
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Jump", true);
}

void Fall_State::Update(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->timer += Time::GetDeltaTime();

    // ���� �Է� ���� 
    if (fsm->GetPlayerFSM()->isAttackIgnore && fsm->GetPlayerFSM()->timer > 0.3f) fsm->GetPlayerFSM()->isAttackIgnore = false;

    // [ Idle ]
    if (fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->GetPlayerFSM()->GetAudioSource()->SetClip(fsm->GetPlayerFSM()->SFX_Player_Land);
        fsm->GetPlayerFSM()->GetAudioSource()->PlayOneShot();

        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Idle_State>());
        return;
    }

    // [ Jump ] �ر� ���� Ȯ��
    if (GameManager::Get().CheckUnlock(SkillType::JumpAttackExtra) &&
        fsm->GetPlayerFSM()->canDoubleJump &&
        !fsm->GetPlayerFSM()->GetIsGround() && 
        !fsm->GetPlayerFSM()->GetIsWall() &&
        fsm->GetPlayerFSM()->GetIsSpace())
    {
        fsm->GetPlayerFSM()->canDoubleJump = false;  // ��� ó��
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Jump_State>());
        return;
    }

    // [ Hanging ]
    if (!fsm->GetPlayerFSM()->GetIsGround() && GameManager::Get().CheckUnlock(SkillType::WallJump) 
        && fsm->GetPlayerFSM()->canHanging)
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

    // [ Attack / Bullet ]
    if (!fsm->GetPlayerFSM()->isAttackIgnore && 
        fsm->GetPlayerFSM()->CanAttack() && 
        fsm->GetPlayerFSM()->GetIsLButton() && 
        !fsm->GetPlayerFSM()->canFallAttack)
    {
        if (!fsm->GetPlayerFSM()->isHolding) { fsm->GetPlayerFSM()->isHolding = true;   fsm->GetPlayerFSM()->holdTime = 0.0f; }

        fsm->GetPlayerFSM()->holdTime += Time::GetDeltaTime();

        // [ BulletTime ]
        if (fsm->GetPlayerFSM()->CanAttack() &&
            fsm->GetPlayerFSM()->holdTime >= fsm->GetPlayerFSM()->bulletTimeThreshold)
        {
            fsm->GetPlayerFSM()->canFallAttack = false;
            fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<BulletTime_State>());
        }
    }
    else if(!fsm->GetPlayerFSM()->isAttackIgnore && !fsm->GetPlayerFSM()->canFallAttack)
    {
        // [ Attack ]
        if (fsm->GetPlayerFSM()->CanAttack() &&
            fsm->GetPlayerFSM()->isHolding && fsm->GetPlayerFSM()->holdTime < fsm->GetPlayerFSM()->bulletTimeThreshold)
        {
            fsm->GetPlayerFSM()->canFallAttack = false;
            fsm->GetPlayerFSM()->OnAirAttack();
            fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Attack_State>());
        }

        // �ʱ�ȭ
        fsm->GetPlayerFSM()->isHolding = false; fsm->GetPlayerFSM()->holdTime = 0.0f;
    }
}

void Fall_State::FixedUpdate(MovementFSM* fsm) 
{
    // ���� �ϰ� 
    if (GameManager::Get().CheckUnlock(SkillType::FastFall) && fsm->GetPlayerFSM()->GetIsS())
    {
        if (!fsm->GetPlayerFSM()->didFastFall)
        {
            // ��� ū �ϰ� �ӵ� �ο�
            fsm->GetPlayerFSM()->GetRigidbody()->velocity.y = fsm->GetPlayerFSM()->fastFallForce;
            fsm->GetPlayerFSM()->didFastFall = true;
        }
        fsm->GetPlayerFSM()->GetRigidbody()->gravityScale = fsm->GetPlayerFSM()->fastFallGravity;
    }

    // [ �¿� �̵� ]
    inputX = fsm->GetPlayerFSM()->GetInputX();
    curVelX = fsm->GetPlayerFSM()->GetRigidbody()->velocity.x;

    // �Է��� �ִ� ��� 
    if (inputX != 0.0f) 
    {
        if (!fsm->GetPlayerFSM()->GetIsWall()) // ���̶� ���� �ʾ��� ���� ���� 
        {
            float targetVelX = inputX * fsm->GetPlayerFSM()->GetCurSpeed();
            fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = Lerp(curVelX, targetVelX, Time::GetDeltaTime() * airAcceleration);
        }
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

    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Jump", false);

    fsm->GetPlayerFSM()->GetAudioSource()->Stop();
}