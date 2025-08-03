#include "Dash_State.h"
#include "Idle_State.h"
#include "MovementFSM.h"
#include "PlayerFSM.h"
#include "Fall_State.h"
#include "PlayerAnimatorController.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"


void Dash_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Dash_State] Dash 시작\n");

    dashTimer = 0.0f;

    // 무적 처리
    fsm->GetPlayerFSM()->SetInvincible(true);

    // 애니메이션
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Jump", true);
}

void Dash_State::Update(MovementFSM* fsm)
{
    dashTimer += Time::GetDeltaTime();

    if (dashTimer >= dashDuration)
    {
        // 무적 해제
        fsm->GetPlayerFSM()->SetInvincible(false);  

        // 상태 전환
        if (fsm->GetPlayerFSM()->GetIsGround())
            fsm->ChangeState(std::make_unique<Idle_State>());
        else
            fsm->ChangeState(std::make_unique<Fall_State>());

        return;
    }
}

void Dash_State::FixedUpdate(MovementFSM* fsm)
{
    float moveBonus = fsm->GetPlayerFSM()->GetMoveSpeedBonus();
    float dashSpeed = (fsm->GetPlayerFSM()->GetWalkSpeed() + moveBonus) * 1.5f; // 150%

    int inputX = fsm->GetPlayerFSM()->GetInputX();
    fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = inputX * dashSpeed;
}

void Dash_State::Exit(MovementFSM* fsm)
{
    OutputDebugStringA("[Dash_State] Dash 종료\n");

    // 애니메이션 종료
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Jump", false);
}