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
    OutputDebugStringA("[Hanging_State] 벽에 매달림 상태 진입\n");

    // 수직 속도를 0으로 (벽에 매달림 효과)
    fsm->GetPlayerFSM()->GetRigidbody()->velocity.y = 0.0f;
    fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = 0.0f;

    fsm->GetPlayerFSM()->SetLastFlipX(!fsm->GetPlayerFSM()->GetLastFlipX());

    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Hanging", true);
}

void Hanging_State::Update(MovementFSM* fsm)
{
    // 점프 키 누르면 벽 점프로 전환
    if (GameManager::Get().CheckUnlock(SkillType::WallJump) && fsm->GetPlayerFSM()->GetIsSpace() )
    {
        fsm->ChangeState(std::make_unique<Jump_Wall_State>());
        return;
    }

    // 벽이 사라지면
    if (!fsm->GetPlayerFSM()->GetIsWallLeft() && !fsm->GetPlayerFSM()->GetIsWallRight())
    {
        fsm->ChangeState(std::make_unique<Idle_State>());
        return;
    }

    // 반대 방향키 입력하면 떨어짐 
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
    // 중력을 약하게 적용 (천천히 내려오기)
    fsm->GetPlayerFSM()->GetRigidbody()->gravityScale = 10;
}

void Hanging_State::Exit(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->GetRigidbody()->gravityScale = 80;

    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Hanging", false);
}
