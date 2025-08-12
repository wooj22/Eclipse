#include "Fall_State.h"
#include "Idle_State.h"
#include "Walk_State.h"
#include "Jump_State.h"
#include "Attack_State.h"
#include "BulletTime_State.h"
#include "Hanging_State.h"
#include "Dash_State.h"

#include "MovementFSM.h" 
#include "PlayerFSM.h"
#include "Player.h"
#include "PlayerAnimatorController.h"
#include "GameManager.h"

#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"



void Fall_State::Enter(MovementFSM* fsm)
{
    OutputDebugStringA("[Fall_State] Player의 Fall_State 진입 \n");

    fsm->GetPlayerFSM()->GetRigidbody()->useGravity = true;
    fsm->GetPlayerFSM()->timer = 0.0f;
    fsm->GetPlayerFSM()->didFastFall = false; 

    // bool isRight = fsm->GetPlayerFSM()->GetLastFlipX(); 
    // if(isRight) fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(10.0f, 0.0f));
    // else fsm->GetPlayerFSM()->GetRigidbody()->AddImpulse(Vector2(-10.0f, 0.0f));

    // 애니메이션 재생
    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Jump", true);
}

void Fall_State::Update(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->timer += Time::GetDeltaTime();

    // 공격 입력 무시 
    if (fsm->GetPlayerFSM()->isAttackIgnore && fsm->GetPlayerFSM()->timer > 0.3f) fsm->GetPlayerFSM()->isAttackIgnore = false;

    // [ Idle ]
    if (fsm->GetPlayerFSM()->GetIsGround())
    {
        fsm->GetPlayerFSM()->GetAudioSource()->SetClip(fsm->GetPlayerFSM()->SFX_Player_Land);
        fsm->GetPlayerFSM()->GetAudioSource()->PlayOneShot();

        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Idle_State>());
        return;
    }

    // [ Jump ] 해금 유무 확인
    if (GameManager::Get().CheckUnlock(SkillType::JumpAttackExtra) &&
        fsm->GetPlayerFSM()->canDoubleJump &&
        !fsm->GetPlayerFSM()->GetIsGround() && 
        !fsm->GetPlayerFSM()->GetIsWall() &&
        fsm->GetPlayerFSM()->GetIsSpace())
    {
        fsm->GetPlayerFSM()->canDoubleJump = false;  // 사용 처리
        fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Jump_State>());
        return;
    }

    // [ Hanging ]
    if (!fsm->GetPlayerFSM()->GetIsGround() && fsm->GetPlayerFSM()->canHanging)
    {
        if (fsm->GetPlayerFSM()->GetIsWallLeft() && fsm->GetPlayerFSM()->GetInputX() < -0.5f)
        {
            fsm->GetPlayerFSM()->canHanging = false;
            fsm->ChangeState(std::make_unique<Hanging_State>());
            return;
        }
        else if (fsm->GetPlayerFSM()->GetIsWallRight() && fsm->GetPlayerFSM()->GetInputX() > 0.5f)
        {
            fsm->GetPlayerFSM()->canHanging = false;
            fsm->ChangeState(std::make_unique<Hanging_State>());
            return;
        }
    }

    // [ Dash ]
    if (fsm->GetPlayerFSM()->GetisShift() && fsm->GetPlayerFSM()->CanDash())
    {
        fsm->ChangeState(std::make_unique<Dash_State>());
        return;
    }

    // [ Attack / Bullet ]
    if (!fsm->GetPlayerFSM()->isAttackIgnore && 
        fsm->GetPlayerFSM()->CanAttack() && 
        fsm->GetPlayerFSM()->GetIsLButton() && 
        !fsm->GetPlayerFSM()->canFallAttack)
    {
        if (!fsm->GetPlayerFSM()->isHolding) { fsm->GetPlayerFSM()->isHolding = true;   fsm->GetPlayerFSM()->holdTime = 0.0f; }

        fsm->GetPlayerFSM()->holdTime += Time::GetDeltaTime();

        // [ BulletTime ]
        if (fsm->GetPlayerFSM()->CanAttack() &&
            fsm->GetPlayerFSM()->holdTime >= fsm->GetPlayerFSM()->bulletTimeThreshold)
        {
            fsm->GetPlayerFSM()->canFallAttack = false;
            fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<BulletTime_State>());
        }
    }
    else if(!fsm->GetPlayerFSM()->isAttackIgnore && !fsm->GetPlayerFSM()->canFallAttack)
    {
        // [ Attack ]
        if (fsm->GetPlayerFSM()->CanAttack() &&
            fsm->GetPlayerFSM()->isHolding && fsm->GetPlayerFSM()->holdTime < fsm->GetPlayerFSM()->bulletTimeThreshold)
        {
            fsm->GetPlayerFSM()->canFallAttack = false;
            fsm->GetPlayerFSM()->OnAirAttack();
            fsm->GetPlayerFSM()->GetMovementFSM()->ChangeState(std::make_unique<Attack_State>());
        }

        // 초기화
        fsm->GetPlayerFSM()->isHolding = false; fsm->GetPlayerFSM()->holdTime = 0.0f;
    }
}

void Fall_State::FixedUpdate(MovementFSM* fsm) 
{
    // 빠른 하강 
    if (GameManager::Get().CheckUnlock(SkillType::FastFall) && fsm->GetPlayerFSM()->GetIsS())
    {
        if (!fsm->GetPlayerFSM()->didFastFall)
        {
            // 즉시 큰 하강 속도 부여
            fsm->GetPlayerFSM()->GetRigidbody()->velocity.y = fsm->GetPlayerFSM()->fastFallForce;
            fsm->GetPlayerFSM()->didFastFall = true;
        }
        fsm->GetPlayerFSM()->GetRigidbody()->gravityScale = fsm->GetPlayerFSM()->fastFallGravity;
    }

    if (fsm->GetPlayerFSM()->didFastFall)
    {
        // 잔상 
        afterimageTimer += Time::GetDeltaTime();
        if (afterimageTimer >= afterimageInterval)
        {
            afterimageTimer = 0.0f;

            CreateAfterImage(fsm);
        }
    }

    // [ 좌우 이동 ]
    inputX = fsm->GetPlayerFSM()->GetInputX();
    curVelX = fsm->GetPlayerFSM()->GetRigidbody()->velocity.x;

    // 입력이 있는 경우 
    if (inputX != 0.0f) 
    {
        if (!fsm->GetPlayerFSM()->GetIsWall()) // 벽이랑 닿지 않았을 때만 보간 
        {
            float targetVelX = inputX * fsm->GetPlayerFSM()->GetCurSpeed();
            fsm->GetPlayerFSM()->GetRigidbody()->velocity.x = Lerp(curVelX, targetVelX, Time::GetDeltaTime() * airAcceleration);
        }
    }
    else
    {
        // 입력이 없으면 감속
        float decelRate = airFriction + (abs(fsm->GetPlayerFSM()->GetRigidbody()->velocity.y) * 0.05f);
        fsm->GetPlayerFSM()->GetRigidbody()->velocity.x =
            Lerp(curVelX, 0.0f, Time::GetDeltaTime() * decelRate);
    }
}

void Fall_State::Exit(MovementFSM* fsm)
{
    fsm->GetPlayerFSM()->GetRigidbody()->gravityScale = fsm->GetPlayerFSM()->defaultGravity;

    fsm->GetPlayerFSM()->GetAnimatorController()->SetBool("Jump", false);

    fsm->GetPlayerFSM()->GetAudioSource()->Stop();
}

void Fall_State::CreateAfterImage(MovementFSM* fsm)
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