#include "Attack_State.h"
#include "Idle_State.h"
#include "MovementFSM.h" 
#include "PlayerFSM.h" 
#include "PlayerAnimatorController.h"

#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/BoxCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Vector2.h"

void Attack_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Attack_State] Player�� Attack_State ���� \n");

    
    if (fsm->GetPlayerFSM()->gameObject->GetComponent<BoxCollider>())
    {
        fsm->GetPlayerFSM()->gameObject->GetComponent<BoxCollider>()->OnDisable_Inner(); OutputDebugStringA("�ڽ� �ݶ��̴��� �����ϴ�! \n");
    }
    else OutputDebugStringA("�ڽ� �ݶ��̴��� �����ϴ�! \n");
    // �ִϸ��̼� ��� 
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Attack", true);

    // ���� ��ġ
    startPos = fsm->GetPlayerFSM()->GetTransform()->GetPosition();
    Vector2 toMouse = fsm->GetPlayerFSM()->MouseWorldPos - startPos;

    // �̵� �Ÿ� ���� (���콺���� �ָ� �� ��)
    float actualDistance = (((toMouse.Magnitude()) < (maxDistance)) ? (toMouse.Magnitude()) : (maxDistance)); // sts::min 
    direction = toMouse.Normalized();
    targetPos = startPos + direction * actualDistance;

    // �ӵ� ���: �Ÿ� / �ð�
    moveSpeed = actualDistance / desiredTime;

}

void Attack_State::Update(MovementFSM* fsm)
{
    timer += Time::GetDeltaTime();

    // [ Idle ] : �ӽ÷� ���� �ð� �� Idle ���·� ��ȯ
    if (timer > 0.5f)
    {
        fsm->GetPlayerFSM()->GetAnimatorController()->PlayAnimation("Samurai_Jump"); // �ӽ� ��ȯ 

        if (fsm->GetPlayerFSM()->GetIsGround())
        {
            fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Idle_State>());
            return;
        }
    }
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
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Idle_State>());
        return;
    }

    // �̵� ����
    fsm->GetPlayerFSM()->GetRigidbody()->velocity = direction * moveSpeed;
}

void Attack_State::Exit(MovementFSM* fsm)
{
    if (fsm->GetPlayerFSM()->GetRigidbody()) fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2::zero;

    if (fsm->GetPlayerFSM()->gameObject->GetComponent<BoxCollider>()) 
    {
        fsm->GetPlayerFSM()->gameObject->GetComponent<BoxCollider>()->OnEnable_Inner(); OutputDebugStringA("�ڽ� �ݶ��̴��� �����ϴ� \n");
    }

    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Attack", false);
}