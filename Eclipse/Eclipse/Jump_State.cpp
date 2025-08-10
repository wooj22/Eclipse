#include "Jump_State.h"
#include "Walk_State.h"
#include "Idle_State.h"
#include "Hanging_State.h"
#include "Jump_Wall_State.h"
#include "BulletTime_State.h"
#include "Attack_State.h"
#include "Dash_State.h"

#include "MovementFSM.h" 
#include "PlayerFSM.h"
#include "PlayerAnimatorController.h"
#include "GameManager.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"



void Jump_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Jump_State] Player�� Jump_State ���� \n");

    // �ʱ�ȭ 
    fsm->GetPlayerFSM()->holdTime = 0.0f;
    fsm->GetPlayerFSM()->isHolding = false;
    fsm->GetPlayerFSM()->timer = 0.0f;
    fsm->GetPlayerFSM()->didFastFall = false; 

    fsm->GetPlayerFSM()->OnJump(JumpPhase::NormalJump);

    // ù��° Jump ���� 
    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = false;
    fsm->GetPlayerFSM()->GetRigidbody()->velocity.y = 0;
    fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(0, fsm->GetPlayerFSM()->GetJumpForce()));
    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = true;

    // �ִϸ��̼� ���
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Jump", true);

    // ����� 
    fsm->GetPlayerFSM()->GetAudioSource()->SetClip(fsm->GetPlayerFSM()->SFX_Player_Jump);
    // fsm->GetPlayerFSM()->GetAudioSource()->SetLoop(false);
    // fsm->GetPlayerFSM()->GetAudioSource()->Play();
    fsm->GetPlayerFSM()->GetAudioSource()->PlayOneShot();
}

void Jump_State::Update(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->timer += Time::GetDeltaTime();

    // �ι�° Jump ���� 
    if (GameManager::Get().CheckUnlock(SkillType::DoubleJump) && fsm->GetPlayerFSM()->canDoubleJump
        && !fsm->GetPlayerFSM()->GetIsGround() && fsm->GetPlayerFSM()->GetIsSpace()
        && !fsm->GetPlayerFSM()->GetIsWall())
    {
        fsm->GetPlayerFSM()->GetRigidbody()->useGravity = false;
        fsm->GetPlayerFSM()->GetRigidbody()->velocity.y = 0;
        fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(0, fsm->GetPlayerFSM()->GetJumpForce()));
        fsm->GetPlayerFSM()->GetRigidbody()->useGravity = true;
        fsm->GetPlayerFSM()->canDoubleJump = false;

        fsm->GetPlayerFSM()->OnJump(JumpPhase::DoubleJump);
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

    // [ Attack / Bullet ]
    if (fsm->GetPlayerFSM()->CanAttack() && fsm->GetPlayerFSM()->GetIsLButton())
    {
        if (!fsm->GetPlayerFSM()->isHolding) { fsm->GetPlayerFSM()->isHolding = true;   fsm->GetPlayerFSM()->holdTime = 0.0f; }

        fsm->GetPlayerFSM()->holdTime += Time::GetDeltaTime();

        // [ BulletTime ]
        if (fsm->GetPlayerFSM()->CanAttack() &&
            fsm->GetPlayerFSM()->holdTime >= fsm->GetPlayerFSM()->bulletTimeThreshold)
        {
            fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<BulletTime_State>());
        }
    }
    else
    {
        // [ Attack ]
        if (fsm->GetPlayerFSM()->CanAttack() &&
            fsm->GetPlayerFSM()->isHolding && fsm->GetPlayerFSM()->holdTime < fsm->GetPlayerFSM()->bulletTimeThreshold)
        {
            fsm->GetPlayerFSM()->OnAirAttack();
            fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Attack_State>());
        }

        // �ʱ�ȭ
        fsm->GetPlayerFSM()->isHolding = false; fsm->GetPlayerFSM()->holdTime = 0.0f;
    }


    // [ Idle ] : ���� �ð� �Ŀ��� ����
    if (fsm->GetPlayerFSM()->GetIsGround() && fsm->GetPlayerFSM()->timer > coyoteTime)
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Idle_State>());
        return;
    }

    // [ Dash ]
    if (fsm->GetPlayerFSM()->GetisShift() && GameManager::Get().CheckUnlock(SkillType::Dash) && fsm->GetPlayerFSM()->CanDash())
    {
        fsm->ChangeState(std::make_unique<Dash_State>());
        return;
    }
}

void Jump_State::FixedUpdate(MovementFSM* fsm)
{
    // [ ���� �ϰ� ]
    if (GameManager::Get().CheckUnlock(SkillType::FastFall) &&
        // fsm->GetPlayerFSM()->GetRigidbody()->velocity.y < 0 && // �ְ��� ���� ����
        fsm->GetPlayerFSM()->GetIsS())
    {
        if (!fsm->GetPlayerFSM()->didFastFall)
        {
            fsm->GetPlayerFSM()->GetRigidbody()->velocity.y = fsm->GetPlayerFSM()->fastFallForce;
            fsm->GetPlayerFSM()->didFastFall = true;
        }
        fsm->GetPlayerFSM()->GetRigidbody()->gravityScale = fsm->GetPlayerFSM()->fastFallGravity;
    }
    else
    {
        fsm->GetPlayerFSM()->GetRigidbody()->gravityScale = fsm->GetPlayerFSM()->defaultGravity;
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

void Jump_State::Exit(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Jump", false);
    fsm->GetPlayerFSM()->GetAudioSource()->Stop();
}