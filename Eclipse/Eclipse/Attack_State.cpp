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
#include "AfterImage.h"

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

    startPos = fsm->GetPlayerFSM()->GetTransform()->GetPosition();   // 현재 위치 
    Vector2 toMouse = fsm->GetPlayerFSM()->MouseWorldPos - startPos; // 목표 위치 

    // 이동 거리 제한 (마우스보다 멀리 못 감)
    float actualDistance = (toMouse.Magnitude() < baseMaxDistance) ? toMouse.Magnitude() : baseMaxDistance;
    direction = toMouse.Normalized();

    // [ 공중 이동 조건 ]
    airAttack = !fsm->GetPlayerFSM()->GetIsGround() || (fsm->GetPlayerFSM()->isBulletAttack && fsm->GetPlayerFSM()->MouseWorldPos.y > startPos.y);

    if (airAttack)
    {
        targetPos = startPos + direction * actualDistance;
        moveSpeed = actualDistance / desiredTime; // 속도 계산 : 거리 / 시간
        fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(0, 5.0f));
        OutputDebugStringA("[Attack_State] 공중 공격 진입 \n");
    }
    else
    {
        // 땅에서 제자리 공격
        targetPos = startPos;
        targetPos = toMouse.Normalized();
        moveSpeed = 0.0f;
        OutputDebugStringA("[Attack_State] 제자리 공격 진입 \n");
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

    // PlayerAttackArea 콜라이더 위치/회전 설정
    attackAreaPosition = startPos + direction * attackAreaDistance;
    fsm->GetPlayerFSM()->GetPlayerAttackArea()->GetComponent<Transform>()->SetPosition(attackAreaPosition);
	fsm->GetPlayerFSM()->GetPlayerAttackArea()->GetComponent<Transform>()->SetRotation(angleDeg + 180.0f); // -90도 회전 적용
   
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
}

void Attack_State::Update(MovementFSM* fsm)
{
    timer += Time::GetDeltaTime();
}

void Attack_State::FixedUpdate(MovementFSM* fsm) 
{
    if (!fsm->GetPlayerFSM()->GetRigidbody() || !fsm->GetPlayerFSM()->GetTransform()) return;

    // 공격 영역 위치 갱신
    Vector2 playerPos = fsm->GetPlayerFSM()->GetTransform()->GetPosition();
    Vector2 toMouse = fsm->GetPlayerFSM()->MouseWorldPos - playerPos;

    if (toMouse.Magnitude() > 0.001f) // 0나누기 방지
    {
        Vector2 direction = toMouse.Normalized();
        Vector2 attackAreaPos = playerPos + direction * attackAreaDistance;

        // 공격 영역 위치 갱신
        fsm->GetPlayerFSM()->GetPlayerAttackArea()->GetComponent<Transform>()->SetPosition(attackAreaPos);
    }

    // 잔상 
    if (airAttack)
    {
        afterimageTimer += Time::GetDeltaTime();
        if (afterimageTimer >= afterimageInterval)
        {
            afterimageTimer = 0.0f;

            CreateAfterImage(fsm);
        }
    }

    if (!isAttackStart)
    {
       fsm->GetPlayerFSM()->GetRigidbody()->velocity = direction * moveSpeed; // 이동
    }

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
    fsm->GetPlayerFSM()->isBulletAttack = false;

    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = true;
    fsm->GetPlayerFSM()->GetRigidbody()->velocity.y = 0;

    fsm->GetPlayerFSM()->GetPlayerAttackArea()->Deactivate();

    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Attack", false);

    fsm->GetPlayerFSM()->GetAudioSource()->Stop();
}


void Attack_State::CreateAfterImage(MovementFSM* fsm)
{
    PlayerFSM* player = fsm->GetPlayerFSM();
    if (!player) return;

    // 현재 스프라이트 가져오기
    SpriteRenderer* playerRenderer = player->GetSpriteRenderer();
    if (!playerRenderer) return;

    shared_ptr<Sprite> currentSprite = playerRenderer->sprite;
    if (!currentSprite || !currentSprite->texture || !currentSprite->texture->texture2D)
    {
        OutputDebugStringA("AfterImage sprite에 texture가 없음!\n");
        return;
    }

    // 위치 및 방향
    Vector2 position = player->GetTransform()->GetPosition();
    bool flipX = playerRenderer->flipX;

    // 잔상 오브젝트 생성 
    GameObject* afterImage = player->Instantiate<GameObject>();
    afterImage->AddComponent<Transform>()->SetScale(player->GetTransform()->GetScale());
    afterImage->GetComponent<Transform>()->SetPosition(position);

    // 렌더러 추가
    auto renderer = afterImage->AddComponent<SpriteRenderer>();
    renderer->sprite = currentSprite;
    renderer->flipX = flipX;
    renderer->flipY = playerRenderer->flipY;
    renderer->SetAlpha(0.1f);
    renderer->SetColor(1.0f, 1.0f, 0.8f);
    renderer->renderMode = RenderMode::Lit_ColorTint;
    renderer->layer = 1;

    // 잔상 스크립트
    auto afterImageScript = afterImage->AddComponent<AfterImage>();
    afterImageScript->SetInitialAlpha(0.4f);
}