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
    OutputDebugStringA("[Attack_State] Player의 Attack_State 진입 \n");

    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = false;
    fsm->GetPlayerFSM()->GetRigidbody()->velocity = Vector2::zero;

    // 초기화 : 스킬 해금 레벨에 따라 보정
    float skillBonus = fsm->GetPlayerFSM()->GetAttackRangeBonus();
    baseMaxDistance = fsm->GetPlayerFSM()->maxAttackDistance * skillBonus;
    desiredTime = fsm->GetPlayerFSM()->attackDesiredTime;

    // 애니메이션 재생 
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Attack", true);

    startPos = fsm->GetPlayerFSM()->GetTransform()->GetPosition(); // 현재 위치 
    Vector2 toMouse = fsm->GetPlayerFSM()->MouseWorldPos - startPos; // 목표 위치

    // 이동 거리 제한 (마우스보다 멀리 못 감)
    float actualDistance = (toMouse.Magnitude() < baseMaxDistance) ? toMouse.Magnitude() : baseMaxDistance;
    direction = toMouse.Normalized();


    if (!fsm->GetPlayerFSM()->GetIsGround()) // 땅에 없을 때만 이동 계산
    {
        targetPos = startPos + direction * actualDistance;

        // 속도 계산 : 거리 / 시간
        moveSpeed = actualDistance / desiredTime;

        std::string dbg = "[Attack_State] Attack desiredTime (Air) : " + std::to_string(desiredTime) + ")\n";
        OutputDebugStringA(dbg.c_str());
    }
    else
    {
        // 땅에 있을 때는 제자리에서 공격
        targetPos = startPos;
        targetPos = toMouse.Normalized();
        moveSpeed = 0.0f;

        std::string dbg = "[Attack_State] Attack desiredTime (Ground) : " + std::to_string(desiredTime) + ")\n";
        OutputDebugStringA(dbg.c_str());
    }


    // 방향 벡터 저장 
    fsm->GetPlayerFSM()->attackDirection = direction;

    // 좌우 반전 처리
    if (direction.x < 0) fsm->GetPlayerFSM()->SetLastFlipX(false);
    else if (direction.x > 0) fsm->GetPlayerFSM()->SetLastFlipX(true);


    // [ 이펙트, 충돌 ]

    // 각도 계산 
    float angleRad = atan2(direction.y, direction.x);
    float angleDeg = angleRad * (180.0f / 3.14159265f);

    // playerAttack_Parent 회전 : ( 원본 이미지가 위쪽 방향 기준 -> 시계방향 -90도 회전 적용 )
    fsm->GetPlayerFSM()->GetPlayerAttackParent()->GetComponent<Transform>()->SetRotation(angleDeg + 180.0f);
   
    // 공격 이펙트 애니메이션 
    auto anim = fsm->GetPlayerFSM()->GetPlayerAttackArea()->GetComponent<Animator>();
    if (anim)
    {
        auto attackAnimCtrl = dynamic_cast<AttackAnimatorController*>(anim->controller);
        if (attackAnimCtrl)
        {
            attackAnimCtrl->PlayAttack();
        }
    }

    // 이펙트 & 범위 활성화
    fsm->GetPlayerFSM()->GetPlayerAttackArea()->Activate();

    // 오디오 
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