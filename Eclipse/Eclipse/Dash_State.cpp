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

    // 물리 초기화 
    fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2(0, 0);
    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = false;
    
    // 대시 
    moveBonus = fsm->GetPlayerFSM()->GetMoveSpeedBonus();
    dashSpeed = (fsm->GetPlayerFSM()->GetWalkSpeed() + moveBonus) * 1.5f; // 150%

    inputX = fsm->GetPlayerFSM()->GetInputX();

    // 입력이 없을 경우 바라보는 방향으로 설정
    if (inputX == 0)
    {
        bool flipX = fsm->GetPlayerFSM()->GetLastFlipX();
        inputX = flipX ? -1 : 1;  // true: 왼쪽 → -1, false: 오른쪽 → +1
    }

    // 애니메이션
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Jump", true);
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
    fsm->GetPlayerFSM()->GetRigidbody()->velocity.y = 0.0f;
    fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = inputX * dashSpeed;
    
    std::string debugStr = "[Dash_State] velocity.y = " + std::to_string(fsm->GetPlayerFSM()->GetRigidbody()->velocity.y) + "\n";
    OutputDebugStringA(debugStr.c_str());

    // std::string debugStr = "[Dash_State] Dash Speed: " + std::to_string(dashSpeed) + "\n";
    // OutputDebugStringA(debugStr.c_str());
}

void Dash_State::Exit(MovementFSM* fsm)
{
    OutputDebugStringA("[Dash_State] Dash 종료\n");

    // 물리 초기화 
    fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2(0, 0);
    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = true;

    // 대시 후 쿨타임 설정
    fsm->GetPlayerFSM()->ResetDashCooldown(); 

    // 애니메이션 종료
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Jump", false);
}