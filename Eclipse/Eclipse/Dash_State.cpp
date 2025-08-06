#include "Dash_State.h"
#include "Idle_State.h"
#include "MovementFSM.h"
#include "PlayerFSM.h"
#include "Fall_State.h"
#include "PlayerAnimatorController.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Time.h"
#include "AfterImage.h"


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
        // inputX = flipX ? -1 : 1;  // true: ���� �� -1, false: ������ �� +1
        inputX = flipX ? 1 : -1;  
    }

    // ���� ��ġ �������� ���� riseAmount ��ŭ ���
    Vector2 currentPos = fsm->GetPlayerFSM()->GetTransform()->GetPosition();
    targetYPosition = currentPos.y - riseAmount; // Y�� ���� ������ ����� -riseAmount

    Vector2 newPos = Vector2(currentPos.x, targetYPosition);
    fsm->GetPlayerFSM()->GetTransform()->SetPosition(newPos);


    // �ִϸ��̼�
    if (!fsm->GetPlayerFSM()->isReleaseSkillAvailable) fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Dash", true);
    else fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Y_Player_Dash", true);

}

void Dash_State::Update(MovementFSM* fsm)
{
    dashTimer += Time::GetDeltaTime();

    if (dashTimer >= dashDuration)
    {
        // ���� ����
        fsm->GetPlayerFSM()->SetInvincible(false);  

        // ���� ��ȯ
        if (fsm->GetPlayerFSM()->GetIsGround()) fsm->ChangeState(std::make_unique<Idle_State>());
        else fsm->ChangeState(std::make_unique<Fall_State>());

        return;
    }
}

void Dash_State::FixedUpdate(MovementFSM* fsm)
{
    // X�� �ӵ��� ���� (Y�� ����)
    fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2(inputX * dashSpeed, 0.0f);

    // Y��ġ ����
    Vector2 currentPos = fsm->GetPlayerFSM()->GetTransform()->GetPosition();
    currentPos.y = targetYPosition;
    fsm->GetPlayerFSM()->GetTransform()->SetPosition(currentPos);

    //std::string debugStr = "[Dash_State] velocity.y = " + std::to_string(fsm->GetPlayerFSM()->GetRigidbody()->velocity.y) + "\n";
    //OutputDebugStringA(debugStr.c_str());

    afterimageTimer += Time::GetDeltaTime();
    if (afterimageTimer >= afterimageInterval)
    {
        afterimageTimer = 0.0f;

        CreateAfterImage(fsm);
    }
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
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Dash", false);
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Y_Player_Dash", false);

    //if (!fsm->GetPlayerFSM()->isReleaseSkillAvailable) fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("N_Player_Dash", false);
    //else fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Y_Player_Dash", false);
}


void Dash_State::CreateAfterImage(MovementFSM* fsm)
{
    PlayerFSM* player = fsm->GetPlayerFSM();
    if (!player) return;

    // ���� ��������Ʈ ��������
    SpriteRenderer* playerRenderer = player->GetSpriteRenderer();
    if (!playerRenderer) return;

    shared_ptr<Sprite> currentSprite = playerRenderer->sprite;
    if (!currentSprite || !currentSprite->texture || !currentSprite->texture->texture2D)
    {
        OutputDebugStringA("AfterImage sprite�� texture�� ����!\n");
        return;
    }

    // ��ġ �� ����
    Vector2 position = player->GetTransform()->GetPosition();
    bool flipX = playerRenderer->flipX;

    // �ܻ� ������Ʈ ���� 
    GameObject* afterImage = player->Instantiate<GameObject>();
    afterImage->AddComponent<Transform>()->SetScale(player->GetTransform()->GetScale());
    afterImage->GetComponent<Transform>()->SetPosition(position);

    // ������ �߰�
    auto renderer = afterImage->AddComponent<SpriteRenderer>();
    renderer->sprite = currentSprite;
    renderer->flipX = flipX;
    renderer->flipY = playerRenderer->flipY;
    renderer->SetAlpha(0.1f);                
    renderer->SetColor(1.0f, 1.0f, 0.8f);
    renderer->renderMode = RenderMode::Lit_ColorTint;
    renderer->layer = 1;

    // �ܻ� ��ũ��Ʈ
    auto afterImageScript = afterImage->AddComponent<AfterImage>();
    afterImageScript->SetInitialAlpha(0.4f);
}
