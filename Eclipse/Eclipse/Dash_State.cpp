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
    OutputDebugStringA("[Dash_State] Dash ����\n");

    dashTimer = 0.0f;

    // ���� ó��
    fsm->GetPlayerFSM()->SetInvincible(true);

    // ���� �ʱ�ȭ 
    fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2(0, 0);
    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = false;
    
    // ��� 
    moveBonus = fsm->GetPlayerFSM()->GetMoveSpeedBonus();
    dashSpeed = (fsm->GetPlayerFSM()->GetWalkSpeed() + moveBonus) * 1.5f; // 150%

    inputX = fsm->GetPlayerFSM()->GetInputX();

    // �Է��� ���� ��� �ٶ󺸴� �������� ����
    if (inputX == 0)
    {
        bool flipX = fsm->GetPlayerFSM()->GetLastFlipX();
        inputX = flipX ? -1 : 1;  // true: ���� �� -1, false: ������ �� +1
    }

    // �ִϸ��̼�
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Jump", true);
}

void Dash_State::Update(MovementFSM* fsm)
{
    dashTimer += Time::GetDeltaTime();

    if (dashTimer >= dashDuration)
    {
        // ���� ����
        fsm->GetPlayerFSM()->SetInvincible(false);  

        // ���� ��ȯ
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
    OutputDebugStringA("[Dash_State] Dash ����\n");

    // ���� �ʱ�ȭ 
    fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2(0, 0);
    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = true;

    // ��� �� ��Ÿ�� ����
    fsm->GetPlayerFSM()->ResetDashCooldown(); 

    // �ִϸ��̼� ����
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Jump", false);
}