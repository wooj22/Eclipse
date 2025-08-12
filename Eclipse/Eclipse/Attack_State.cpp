#include "Attack_State.h"
#include "Idle_State.h"
#include "MovementFSM.h" 
#include "PlayerFSM.h" 
#include "PlayerAnimatorController.h"

#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/BoxCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Vector2.h"

#include "Fall_State.h"
#include "../Direct2D_EngineLib/Input.h"

void Attack_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Attack_State] Player�� Attack_State ���� \n");

    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = false;
    fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2::zero;

    // �ʱ�ȭ : ��ų �ر� ������ ���� ����
    float skillBonus = fsm->GetPlayerFSM()->GetAttackRangeBonus();
    baseMaxDistance = fsm->GetPlayerFSM()->maxAttackDistance * skillBonus;
    desiredTime = fsm->GetPlayerFSM()->attackDesiredTime;

    // �ִϸ��̼� ��� 
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Attack", true);

    startPos = fsm->GetPlayerFSM()->GetTransform()->GetPosition(); // ���� ��ġ 
    Vector2 toMouse = fsm->GetPlayerFSM()->MouseWorldPos - startPos; // ��ǥ ��ġ

    // �̵� �Ÿ� ���� (���콺���� �ָ� �� ��)
    float actualDistance = (toMouse.Magnitude() < baseMaxDistance) ? toMouse.Magnitude() : baseMaxDistance;
    direction = toMouse.Normalized();


    if (!fsm->GetPlayerFSM()->GetIsGround()) // ���� ���� ���� �̵� ���
    {
        targetPos = startPos + direction * actualDistance;

        // �ӵ� ��� : �Ÿ� / �ð�
        moveSpeed = actualDistance / desiredTime;

        std::string dbg = "[Attack_State] Attack desiredTime (Air) : " + std::to_string(desiredTime) + ")\n";
        OutputDebugStringA(dbg.c_str());
    }
    else
    {
        // ���� ���� ���� ���ڸ����� ����
        targetPos = startPos;
        targetPos = toMouse.Normalized();
        moveSpeed = 0.0f;

        std::string dbg = "[Attack_State] Attack desiredTime (Ground) : " + std::to_string(desiredTime) + ")\n";
        OutputDebugStringA(dbg.c_str());
    }


    // ���� ���� ���� 
    fsm->GetPlayerFSM()->attackDirection = direction;

    // �¿� ���� ó��
    if (direction.x < 0) fsm->GetPlayerFSM()->SetLastFlipX(false);
    else if (direction.x > 0) fsm->GetPlayerFSM()->SetLastFlipX(true);


    // [ ����Ʈ, �浹 ]

    // ���� ��� 
    float angleRad = atan2(direction.y, direction.x);
    float angleDeg = angleRad * (180.0f / 3.14159265f);

    // playerAttack_Parent ȸ�� : ( ���� �̹����� ���� ���� ���� -> �ð���� -90�� ȸ�� ���� )
    fsm->GetPlayerFSM()->GetPlayerAttackParent()->GetComponent<Transform>()->SetRotation(angleDeg + 180.0f);
   
    // ���� ����Ʈ �ִϸ��̼� 
    auto anim = fsm->GetPlayerFSM()->GetPlayerAttackArea()->GetComponent<Animator>();
    if (anim)
    {
        auto attackAnimCtrl = dynamic_cast<AttackAnimatorController*>(anim->controller);
        if (attackAnimCtrl)
        {
            attackAnimCtrl->PlayAttack();
        }
    }

    // ����Ʈ & ���� Ȱ��ȭ
    fsm->GetPlayerFSM()->GetPlayerAttackArea()->Activate();

    // ����� 
    fsm->GetPlayerFSM()->GetAudioSource()->SetClip(fsm->GetPlayerFSM()->SFX_Player_Attack);
    fsm->GetPlayerFSM()->GetAudioSource()->PlayOneShot();

    fsm->GetPlayerFSM()->GetRigidbody()->velocity = direction * moveSpeed;
}

void Attack_State::Update(MovementFSM* fsm)
{
    timer += Time::GetDeltaTime();
}

void Attack_State::FixedUpdate(MovementFSM* fsm)
{
    if (!fsm->GetPlayerFSM()->GetRigidbody() || !fsm->GetPlayerFSM()->GetTransform()) return;

    if (timer >= desiredTime)
    {
        if (fsm->GetPlayerFSM()->GetIsGround()) { fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Idle_State>()); return; }
        else { fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Fall_State>()); return; }
    }

    // if (direction == Vector2::zero) return;

    Vector2 currentPos = fsm->GetPlayerFSM()->GetTransform()->GetPosition();
    float traveled = (currentPos - startPos).Magnitude(); 
    float totalDistance = (targetPos - startPos).Magnitude(); 

    // ���� ���� ����
    if (traveled >= totalDistance)
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Fall_State>());
        return;
    }
    else if (fsm->GetPlayerFSM()->GetIsGround() || fsm->GetPlayerFSM()->GetIsWallLeft() || fsm->GetPlayerFSM()->GetIsWallRight())
    {
        // �������� ���ߴµ�, �� or �� �̶� �浹���� �� 
        fsm->GetPlayerFSM()->GetRigidbody()->useGravity = false;
        fsm->GetPlayerFSM()->GetRigidbody()->velocity.y = 0;
        fsm->GetPlayerFSM()->GetRigidbody()->useGravity = true;
    }
}

void Attack_State::Exit(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->OnAirAttack();

    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = true;
    fsm->GetPlayerFSM()->GetRigidbody()->velocity.y = 0;
    // fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2::zero;

    fsm->GetPlayerFSM()->GetPlayerAttackArea()->Deactivate();

    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Attack", false);

    fsm->GetPlayerFSM()->GetAudioSource()->Stop();
}