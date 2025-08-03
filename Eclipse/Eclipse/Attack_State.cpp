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
    OutputDebugStringA("[Attack_State] Player의 Attack_State 진입 \n");

    // 애니메이션 재생 
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Attack", true);

    // [ 공격 이동 ] 
    startPos = fsm->GetPlayerFSM()->GetTransform()->GetPosition(); // 시작 위치
    Vector2 toMouse = fsm->GetPlayerFSM()->MouseWorldPos - startPos; // 목표 위치 

    // 이동 거리 제한 (마우스보다 멀리 못 감)
    float actualDistance = (((toMouse.Magnitude()) < (maxDistance)) ? (toMouse.Magnitude()) : (maxDistance)); // sts::min 
    direction = toMouse.Normalized();
    targetPos = startPos + direction * actualDistance;

    // 속도 계산: 거리 / 시간
    moveSpeed = actualDistance / desiredTime;


    // [ 이펙트, 충돌 ]

    // 각도 계산 
    float angleRad = atan2(direction.y, direction.x);
    float angleDeg = angleRad * (180.0f / 3.14159265f);

    // playerAttack_Parent 회전 : ( 원본 이미지가 위쪽 방향 기준 -> 시계방향 -90도 회전 적용 )
    fsm->GetPlayerFSM()->GetPlayerAttackParent()->GetComponent<Transform>()->SetRotation(angleDeg - 90.0f);

    // 활성화
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

    // 도착 여부 판정
    if (traveled >= totalDistance)
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Fall_State>());
        return;
    }
    else if (fsm->GetPlayerFSM()->GetIsGround() || fsm->GetPlayerFSM()->GetIsWallLeft() || fsm->GetPlayerFSM()->GetIsWallRight())
    {
        // 도착하지 못했는데, 땅 or 벽 이랑 충돌했을 때 
        fsm->GetPlayerFSM()->GetRigidbody()->useGravity = false;
        fsm->GetPlayerFSM()->GetRigidbody()->velocity.y = 0;
        fsm->GetPlayerFSM()->GetRigidbody()->useGravity = true;
    }
    
    if( timer >= desiredTime)
    {
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Fall_State>());
        return;
    }

    // 이동 유지
    fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(direction * moveSpeed);
}

void Attack_State::Exit(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->UseAttack(); // 공격 차감 

    if (fsm->GetPlayerFSM()->GetRigidbody()) fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2::zero;

    fsm->GetPlayerFSM()->GetPlayerAttackArea()->SetActive(false);

    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Samurai_Attack", false);
}