#include "Idle_State.h"
#include "Walk_State.h"
#include "Jump_State.h"
#include "MovementFSM.h" 
#include "PlayerFSM.h"
#include "PlayerAnimatorController.h"
#include "Attack_State.h"
#include "BulletTime_State.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"

void Idle_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Idle_State] Player의 Idle_State 진입 \n");

    // 초기화 
    holdTime = 0.0f;
    isHolding = false;
    timer = 0.0f;

    fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = 0.0f;         // 움직임이 있었다면 정지 

    // 애니메이션 재생
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Idle", true);
}

void Idle_State::Update(MovementFSM* fsm)
{
    timer += Time::GetDeltaTime();

    // [ Jump ]
    if (fsm->GetPlayerFSM()->GetIsSpace() && fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Jump_State>());
    }

    // [ Walk ]
    if (fsm->GetPlayerFSM()->GetIsA() || fsm->GetPlayerFSM()->GetIsD())
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Walk_State>());
    }

    // [ Attack ]
    if (Input::GetKey(VK_LBUTTON))
    {
        if (!isHolding)
        {
            isHolding = true;
            holdTime = 0.0f;
        }

        holdTime += Time::GetDeltaTime();

        // [ BulletTime ]
        if (holdTime >= bulletTimeThreshold)                fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<BulletTime_State>());

    }
    else
    {
        // [ Attack ]
        if (isHolding && holdTime < bulletTimeThreshold)    fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Attack_State>());

        // 초기화
        isHolding = false;
        holdTime = 0.0f;
    }
}


void Idle_State::FixedUpdate(MovementFSM* fsm) {}


void Idle_State::Exit(MovementFSM* fsm)
{
   fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Idle", false);
}