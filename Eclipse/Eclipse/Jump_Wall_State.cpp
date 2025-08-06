#include "Jump_Wall_State.h"
#include "Hanging_State.h"
#include "Idle_State.h"
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



void Jump_Wall_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Jump_Wall_State] 벽 점프 상태 진입\n");
 
    // 초기화 
    fsm->GetPlayerFSM()->holdTime = 0.0f;
    fsm->GetPlayerFSM()->isHolding = false;
    fsm->GetPlayerFSM()->timer = 0.0f;
    fsm->GetPlayerFSM()->OnJump(JumpPhase::WallJump);

    elapsedTime = 0.0f;  // 시간
    wallJumpForce = fsm->GetPlayerFSM()->GetJumpForce();
    fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2(0, 0); // 수직 속도


    // 첫번째 Wall Jump 실행 
    if (fsm->GetPlayerFSM()->GetIsWallLeft())
    {
        fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(+jumpXPower, wallJumpForce));  // 오른쪽 방향으로 튕김
        lastWallDir = -1;
    }
    else if (fsm->GetPlayerFSM()->GetIsWallRight())
    {
        fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(-jumpXPower, wallJumpForce));  // 왼쪽 방향으로 튕김
        lastWallDir = 1;
    }
    else
    {
        lastWallDir = 0;
    }

    // 애니메이션 재생
    if (!fsm->GetPlayerFSM()->isReleaseSkillAvailable) fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Jump", true);
    else fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Y_Player_Jump", true);
}

void Jump_Wall_State::Update(MovementFSM* fsm)
{
    // 두번째 Wall Jump 실행 
    if (GameManager::Get().CheckUnlock(SkillType::DoubleJump) && fsm->GetPlayerFSM()->canDoubleJump && fsm->GetPlayerFSM()->GetIsSpace())
    {
        if (!fsm->GetPlayerFSM()->GetIsGround() && !fsm->GetPlayerFSM()->GetLastFlipX())
        {
            fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(+doubleJumpXPower, wallJumpForce));
        }
        else if (!fsm->GetPlayerFSM()->GetIsGround() && fsm->GetPlayerFSM()->GetLastFlipX())
        {
            fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(-doubleJumpXPower, wallJumpForce));
        }

        OutputDebugStringA(" 더블 점프 합니다 \n");
        fsm->GetPlayerFSM()->OnJump(JumpPhase::DoubleJump);
        fsm->GetPlayerFSM()->canDoubleJump = false;
    }

    // [ Hanging ] 
    // 왼쪽으로 튕긴 후, 왼쪽 이동 중이면, 왼쪽벽에 매달리기 
    if (lastWallDir == 1 && fsm->GetPlayerFSM()->GetIsWallLeft() && fsm->GetPlayerFSM()->GetInputX() < -0.5f)
    {
        fsm->ChangeState(std::make_unique<Hanging_State>());
        return;
    }
    else if (lastWallDir == -1 && fsm->GetPlayerFSM()->GetIsWallRight() && fsm->GetPlayerFSM()->GetInputX() > 0.5f)
    {
        fsm->ChangeState(std::make_unique<Hanging_State>());
        return;
    }


    // [ Attack / Bullet ]
    if (fsm->GetPlayerFSM()->CanAttack() && Input::GetKey(VK_LBUTTON))
    {
        if (!fsm->GetPlayerFSM()->isHolding) { fsm->GetPlayerFSM()->isHolding = true;   fsm->GetPlayerFSM()->holdTime = 0.0f; }

        fsm->GetPlayerFSM()->holdTime += Time::GetDeltaTime();

        // [ BulletTime ]
        if ( fsm->GetPlayerFSM()->CanAttack() &&
            (fsm->GetPlayerFSM()->holdTime >= fsm->GetPlayerFSM()->bulletTimeThreshold))
        {
            fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<BulletTime_State>());
        }
    }
    else
    {
        // [ Attack ]
        if ( fsm->GetPlayerFSM()->CanAttack() &&
            (fsm->GetPlayerFSM()->isHolding && fsm->GetPlayerFSM()->holdTime < fsm->GetPlayerFSM()->bulletTimeThreshold))
        {
            fsm->GetPlayerFSM()->OnAirAttack();
            fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Attack_State>());
        }

        // 초기화
        fsm->GetPlayerFSM()->isHolding = false; fsm->GetPlayerFSM()->holdTime = 0.0f;
    }


    // [ Idle ]
    if (fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->ChangeState(std::make_unique<Idle_State>());
        return;
    }

    // [ Dash ]
    if (fsm->GetPlayerFSM()->GetisShift() && GameManager::Get().CheckUnlock(SkillType::Dash) && fsm->GetPlayerFSM()->CanDash())
    {
        fsm->ChangeState(std::make_unique<Dash_State>());
        return;
    }
}

void Jump_Wall_State::FixedUpdate(MovementFSM* fsm)
{
    elapsedTime += Time::GetDeltaTime();

    if (elapsedTime < inputBlockTime) return;

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

    // 입력이 있는 경우: 목표 속도로 보간 ()
    if (inputX != 0.0f)
    {
        curVelX = inputX * fsm->GetPlayerFSM()->GetCurSpeed();
        fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = Lerp(curVelX, curVelX, Time::GetDeltaTime() * airAcceleration);
    }
    else
    {
        // 입력 없으면 감속
        fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = Lerp(curVelX, 0.0f, Time::GetDeltaTime() * airFriction);
    }
}

void Jump_Wall_State::Exit(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Jump", false);
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Y_Player_Jump", false);

    //if (!fsm->GetPlayerFSM()->isReleaseSkillAvailable) fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Jump", false);
    //else fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Y_Player_Jump", false);
}