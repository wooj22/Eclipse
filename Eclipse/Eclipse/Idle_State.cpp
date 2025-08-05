#include "Idle_State.h"
#include "Walk_State.h"
#include "Jump_State.h"
#include "Attack_State.h"
#include "BulletTime_State.h"
#include "Fall_State.h"
#include "Dash_State.h"

#include "MovementFSM.h" 
#include "PlayerFSM.h"
#include "PlayerAnimatorController.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"


void Idle_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Idle_State] Player의 Idle_State 진입 \n");

    // 초기화 
    fsm->GetPlayerFSM()->holdTime = 0.0f;
    fsm->GetPlayerFSM()->isHolding = false;
    // fsm->GetPlayerFSM()->timer = 0.0f;

    fsm->GetPlayerFSM()->OnGround();  // 모든 공격 기회 리셋

    fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2(0, 0);  // 움직임이 있었다면 정지 

    // 애니메이션 재생
    if(!fsm->GetPlayerFSM()->isReleaseSkillAvailable) fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Idle", true);
    else { fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Y_Player_Idle", true); }
}

void Idle_State::Update(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->timer += Time::GetDeltaTime();

    // DubleJump / Hanging 초기화
    if (fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->GetPlayerFSM()->canDoubleJump = true;
        fsm->GetPlayerFSM()->canHanging = true;
        fsm->GetPlayerFSM()->canFallAttack = true;
    }

    // [ Jump ]
    if (fsm->GetPlayerFSM()->GetIsSpace() && fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->GetPlayerFSM()->OnJump(JumpPhase::NormalJump);
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Jump_State>());
    }

    // [ Walk ]
    if (fsm->GetPlayerFSM()->GetIsA() || fsm->GetPlayerFSM()->GetIsD())
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Walk_State>());
    }

    // [ Attack / Bullet ]
    if (fsm->GetPlayerFSM()->GetIsGround() && Input::GetKey(VK_LBUTTON))
    {
        if (!fsm->GetPlayerFSM()->isHolding) { fsm->GetPlayerFSM()->isHolding = true;   fsm->GetPlayerFSM()->holdTime = 0.0f;  }

        fsm->GetPlayerFSM()->holdTime += Time::GetDeltaTime();

        // [ BulletTime ]
        if (fsm->GetPlayerFSM()->holdTime >= fsm->GetPlayerFSM()->bulletTimeThreshold) fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<BulletTime_State>());
    }
    else
    {
        // [ Attack ]
        if (fsm->GetPlayerFSM()->isHolding && fsm->GetPlayerFSM()->holdTime < fsm->GetPlayerFSM()->bulletTimeThreshold) fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Attack_State>());

        // 초기화
        fsm->GetPlayerFSM()->isHolding = false; fsm->GetPlayerFSM()->holdTime = 0.0f;
    }


    // [ Fall ]
    if (!fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Fall_State>());
    }

    // [ Dash ]
    if (fsm->GetPlayerFSM()->GetisShift() && GameManager::Get().CheckUnlock(SkillType::Dash) && fsm->GetPlayerFSM()->CanDash())
    {
        fsm->ChangeState(std::make_unique<Dash_State>());
        return;
    }
}


void Idle_State::FixedUpdate(MovementFSM* fsm) {}


void Idle_State::Exit(MovementFSM* fsm)
{
   if (!fsm->GetPlayerFSM()->isReleaseSkillAvailable) fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Idle", false);
   else fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Y_Player_Idle", false);
}