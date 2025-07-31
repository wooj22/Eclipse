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
    OutputDebugStringA("[Attack_State] Player의 Attack_State 진입 \n");

    
    if (fsm->GetPlayerFSM()->gameObject->GetComponent<BoxCollider>())
    {
        fsm->GetPlayerFSM()->gameObject->GetComponent<BoxCollider>()->OnDisable_Inner(); OutputDebugStringA("박스 콜라이더가 꺼집니다! \n");
    }
    else OutputDebugStringA("박스 콜라이더가 없습니다! \n");
    // 애니메이션 재생 
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Attack", true);

    // 시작 위치
    startPos = fsm->GetPlayerFSM()->GetTransform()->GetPosition();
    Vector2 toMouse = fsm->GetPlayerFSM()->MouseWorldPos - startPos;

    // 이동 거리 제한 (마우스보다 멀리 못 감)
    float actualDistance = (((toMouse.Magnitude()) < (maxDistance)) ? (toMouse.Magnitude()) : (maxDistance)); // sts::min 
    direction = toMouse.Normalized();
    targetPos = startPos + direction * actualDistance;

    // 속도 계산: 거리 / 시간
    moveSpeed = actualDistance / desiredTime;

}

void Attack_State::Update(MovementFSM* fsm)
{
    timer += Time::GetDeltaTime();

    // [ Idle ] : 임시로 일정 시간 후 Idle 상태로 전환
    if (timer > 0.5f)
    {
        fsm->GetPlayerFSM()->GetAnimatorController()->PlayAnimation("Samurai_Jump"); // 임시 전환 

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

    // 도착 여부 판정
    if (traveled >= totalDistance)
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Idle_State>());
        return;
    }

    // 이동 유지
    fsm->GetPlayerFSM()->GetRigidbody()->velocity = direction * moveSpeed;
}

void Attack_State::Exit(MovementFSM* fsm)
{
    if (fsm->GetPlayerFSM()->GetRigidbody()) fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2::zero;

    if (fsm->GetPlayerFSM()->gameObject->GetComponent<BoxCollider>()) 
    {
        fsm->GetPlayerFSM()->gameObject->GetComponent<BoxCollider>()->OnEnable_Inner(); OutputDebugStringA("박스 콜라이더가 켜집니다 \n");
    }

    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Attack", false);
}