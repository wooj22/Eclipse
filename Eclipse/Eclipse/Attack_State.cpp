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

void Attack_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Attack_State] Player�� Attack_State ���� \n");

    // �ִϸ��̼� ��� 
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Attack", true);

    // [ ���� �̵� ] 
    startPos = fsm->GetPlayerFSM()->GetTransform()->GetPosition(); // ���� ��ġ
    Vector2 toMouse = fsm->GetPlayerFSM()->MouseWorldPos - startPos; // ��ǥ ��ġ 

    // �̵� �Ÿ� ���� (���콺���� �ָ� �� ��)
    float actualDistance = (((toMouse.Magnitude()) < (maxDistance)) ? (toMouse.Magnitude()) : (maxDistance)); // sts::min 
    direction = toMouse.Normalized();
    targetPos = startPos + direction * actualDistance;

    // �ӵ� ���: �Ÿ� / �ð�
    moveSpeed = actualDistance / desiredTime;


    // [ ����Ʈ, �浹 ]

    // ���� ��� 
    float angleRad = atan2(direction.y, direction.x);
    float angleDeg = angleRad * (180.0f / 3.14159265f);

    // playerAttack_Parent ȸ�� : ( ���� �̹����� ���� ���� ���� -> �ð���� -90�� ȸ�� ���� )
    fsm->GetPlayerFSM()->GetPlayerAttackParent()->GetComponent<Transform>()->SetRotation(angleDeg - 90.0f);

    // Ȱ��ȭ
    fsm->GetPlayerFSM()->GetPlayerAttackArea()->SetActive(true);
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
    fsm->GetPlayerFSM()->UseAttack(); // ���� ���� 

    if (fsm->GetPlayerFSM()->GetRigidbody()) fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2::zero;

    fsm->GetPlayerFSM()->GetPlayerAttackArea()->SetActive(false);

    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Attack", false);
}