#include "Fall_State.h"
#include "Idle_State.h"
#include "Walk_State.h"
#include "Jump_State.h"
#include "Attack_State.h"
#include "BulletTime_State.h"

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
    OutputDebugStringA("[Fall_State] Player의 Fall_State 진입 \n");

    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = true;

    // 애니메이션 재생
    // fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Fall", true);
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Jump", true);
}

void Fall_State::Update(MovementFSM* fsm)
{
    if (fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Idle_State>());
        return;
    }
}

void Fall_State::FixedUpdate(MovementFSM* fsm) 
{
    // 빠른 하강 
    if (GameManager::Get().CheckUnlock(SkillType::FastFall) && fsm->GetPlayerFSM()->GetIsS())
    {
        fsm->GetPlayerFSM()->GetRigidbody()->gravityScale = fsm->GetPlayerFSM()->fastFallGravity;
    }
}

void Fall_State::Exit(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->GetRigidbody()->gravityScale = fsm->GetPlayerFSM()->defaultGravity;
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Jump", false);
}