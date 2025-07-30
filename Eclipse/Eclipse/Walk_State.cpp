#include "Jump_State.h"
#include "Walk_State.h"
#include "Idle_State.h"
#include "MovementFSM.h" 
#include "PlayerFSM.h"
#include "PlayerAnimatorController.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Time.h"
#include "BulletTime_State.h"
#include "Attack_State.h"

void Walk_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Walk_State] Player의 Walk_State 진입 \n");

    // 초기화 
    fsm->GetPlayerFSM()->holdTime = 0.0f;
    fsm->GetPlayerFSM()->isHolding = false;
    fsm->GetPlayerFSM()->timer = 0.0f;

    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Walk", true);
}

void Walk_State::Update(MovementFSM* fsm)
{
    // [ Jump ]
    if (fsm->GetPlayerFSM()->GetIsSpace() && fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Jump_State>());
    }

    // [ Idle ]
    if (!(fsm->GetPlayerFSM()->GetIsA() || fsm->GetPlayerFSM()->GetIsD()))
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Idle_State>());
    }

    // [ Attack / Bullet ]
    if (Input::GetKey(VK_LBUTTON))
    {
        if (!fsm->GetPlayerFSM()->isHolding) { fsm->GetPlayerFSM()->isHolding = true;   fsm->GetPlayerFSM()->holdTime = 0.0f; }

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
}

void Walk_State::FixedUpdate(MovementFSM* fsm)
{
    // Move 실행 
    fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = fsm->GetPlayerFSM()->GetInputX() * fsm->GetPlayerFSM()->GetCurSpeed();

    
}

void Walk_State::Exit(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Walk", false);
}