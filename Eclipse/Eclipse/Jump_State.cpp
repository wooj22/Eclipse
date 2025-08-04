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
    OutputDebugStringA("[Jump_State] Player의 Jump_State 진입 \n");

    // 초기화 
    // fsm->GetPlayerFSM()->canDoubleJump = true;
    fsm->GetPlayerFSM()->holdTime = 0.0f;
    fsm->GetPlayerFSM()->isHolding = false;
    fsm->GetPlayerFSM()->timer = 0.0f;
    fsm->GetPlayerFSM()->OnJump(JumpPhase::NormalJump);

    // 첫번째 Jump 실행 
    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = false;
    fsm->GetPlayerFSM()->GetRigidbody()->velocity.y = 0;
    fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(0, fsm->GetPlayerFSM()->GetJumpForce()));
    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = true;

    // 애니메이션 재생
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Jump", true);
}

void Jump_State::Update(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->timer += Time::GetDeltaTime();

    // [ Jump_Wall ]
    //if (!fsm->GetPlayerFSM()->GetIsGround() && fsm->GetPlayerFSM()->GetIsSpace())
    //{
    //    if (fsm->GetPlayerFSM()->GetIsWallLeft() || fsm->GetPlayerFSM()->GetIsWallRight())
    //    {
    //        fsm->ChangeState(std::make_unique<Jump_Wall_State>());
    //        return;
    //    }
    //}

    // 두번째 Jump 실행 
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
    if (!fsm->GetPlayerFSM()->GetIsGround() && GameManager::Get().CheckUnlock(SkillType::WallJump))
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

    // [ Attack / Bullet ]
    if (Input::GetKey(VK_LBUTTON))
    {
        if (!fsm->GetPlayerFSM()->isHolding) { fsm->GetPlayerFSM()->isHolding = true;   fsm->GetPlayerFSM()->holdTime = 0.0f; }

        fsm->GetPlayerFSM()->holdTime += Time::GetDeltaTime();

        // [ BulletTime ]
        if (fsm->GetPlayerFSM()->CanAttack() &&
            fsm->GetPlayerFSM()->holdTime >= fsm->GetPlayerFSM()->bulletTimeThreshold)
        {
            // fsm->GetPlayerFSM()->UseAttack();  // 공격 기회 사용
            fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<BulletTime_State>());
        }
    }
    else
    {
        // [ Attack ]
        if (fsm->GetPlayerFSM()->CanAttack() &&
            fsm->GetPlayerFSM()->isHolding && fsm->GetPlayerFSM()->holdTime < fsm->GetPlayerFSM()->bulletTimeThreshold)
        {
            // fsm->GetPlayerFSM()->UseAttack();  // 공격 기회 사용
            fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Attack_State>());
        }

        // 초기화
        fsm->GetPlayerFSM()->isHolding = false; fsm->GetPlayerFSM()->holdTime = 0.0f;
    }


    // [ Idle ] : 일정 시간 후에만 감지
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
    // [ 빠른 하강 ]
    if (GameManager::Get().CheckUnlock(SkillType::FastFall) &&
        fsm->GetPlayerFSM()->GetRigidbody()->velocity.y < 0 && // 최고점 도달 이후
        fsm->GetPlayerFSM()->GetIsS())
    {
        fsm->GetPlayerFSM()->GetRigidbody()->gravityScale = fsm->GetPlayerFSM()->fastFallGravity;
    }
    else
    {
        fsm->GetPlayerFSM()->GetRigidbody()->gravityScale = fsm->GetPlayerFSM()->defaultGravity;
    }


    // [ 좌우 이동 ]
    inputX = fsm->GetPlayerFSM()->GetInputX();
    curVelX = fsm->GetPlayerFSM()->GetRigidbody()->velocity.x;

    // 입력이 있는 경우: 목표 속도로 보간
    if (inputX != 0.0f)
    {
        float targetVelX = inputX * fsm->GetPlayerFSM()->GetCurSpeed();
        fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = Lerp(curVelX, targetVelX, Time::GetDeltaTime() * airAcceleration);
    }
    else
    {
        // 입력이 없으면 감속
        fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = Lerp(curVelX, 0.0f, Time::GetDeltaTime() * airFriction);
    }

}

void Jump_State::Exit(MovementFSM* fsm)
{
    //if (fsm->GetPlayerFSM()->GetIsGround())
    //{
    //    canDoubleJump = true;  // 착지 시 더블 점프를 할 수 있도록 설정
    //}

    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Jump", false);
}