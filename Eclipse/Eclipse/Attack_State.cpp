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

    //std::string dbg = "[Attack_State] Attack MaxDist: " + std::to_string(baseMaxDistance) + "\n";
    //OutputDebugStringA(dbg.c_str());

    // �ִϸ��̼� ��� 
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Attack", true);

    // [ ���� �̵� ] 
    startPos = fsm->GetPlayerFSM()->GetTransform()->GetPosition(); // ���� ��ġ
    Vector2 toMouse = fsm->GetPlayerFSM()->MouseWorldPos - startPos; // ��ǥ ��ġ 

    // �̵� �Ÿ� ���� (���콺���� �ָ� �� ��)
    float actualDistance = (((toMouse.Magnitude()) < (baseMaxDistance)) ? (toMouse.Magnitude()) : (baseMaxDistance)); // sts::min 
	direction = toMouse.Normalized(); // ���� ���� 
    targetPos = startPos + direction * actualDistance;

    // �ӵ� ���: �Ÿ� / �ð�
    moveSpeed = actualDistance / desiredTime;

    // ���� ���� ���� 
    fsm->GetPlayerFSM()->attackDirection = direction;

    Vector2 dir = fsm->GetPlayerFSM()->attackDirection;
    std::string dbg = "[Attack_State] AttackDirection: (" + std::to_string(dir.x) + ", " + std::to_string(dir.y) + ")\n";
    OutputDebugStringA(dbg.c_str());


    // [ ����Ʈ, �浹 ]

    // ���� ��� 
    float angleRad = atan2(direction.y, direction.x);
    float angleDeg = angleRad * (180.0f / 3.14159265f);

    // playerAttack_Parent ȸ�� : ( ���� �̹����� ���� ���� ���� -> �ð���� -90�� ȸ�� ���� )
    fsm->GetPlayerFSM()->GetPlayerAttackParent()->GetComponent<Transform>()->SetRotation(angleDeg + 180.0f);
   
    // ���� ���� Ȱ��ȭ
    fsm->GetPlayerFSM()->GetPlayerAttackArea()->GetComponent<SpriteRenderer>()->SetEnabled(true); 
    fsm->GetPlayerFSM()->GetPlayerAttackArea()->GetComponent<CircleCollider>()->SetEnabled(true); 


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
}

void Attack_State::Update(MovementFSM* fsm)
{
    timer += Time::GetDeltaTime();
}

void Attack_State::FixedUpdate(MovementFSM* fsm)
{
    if (!fsm->GetPlayerFSM()->GetRigidbody() || !fsm->GetPlayerFSM()->GetTransform()) 
    { OutputDebugStringA("!fsm->GetPlayerFSM()->GetRigidbody() || !fsm->GetPlayerFSM()->GetTransform() \n"); return; }

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
    
    if( timer >= desiredTime)
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Fall_State>());
        return;
    }

    // �̵� ����
    fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(direction * moveSpeed);
}

void Attack_State::Exit(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->OnAirAttack();

    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = true;
    fsm->GetPlayerFSM()->GetRigidbody()->velocity.y = 0;
    // fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2::zero;

    fsm->GetPlayerFSM()->GetPlayerAttackArea()->Deactivate();

    //fsm->GetPlayerFSM()->GetPlayerAttackArea()->GetComponent<SpriteRenderer>()->SetEnabled(false); 
    //fsm->GetPlayerFSM()->GetPlayerAttackArea()->GetComponent<CircleCollider>()->SetEnabled(false); 

    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Attack", false);

    fsm->GetPlayerFSM()->GetAudioSource()->Stop();
}