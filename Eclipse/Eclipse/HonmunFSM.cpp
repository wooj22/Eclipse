#include "HonmunFSM.h"
#include "Honmun.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include <memory>

// HonmunIdleState Implementation
void HonmunIdleState::Enter(HonmunFSM* fsm)
{
    idleTimer = 0.0f;
    
    // 정적 스프라이트 사용 중이므로 애니메이션 시작 불필요
    char debugMsg[50];
    sprintf_s(debugMsg, "Honmun type %d entered idle state\n", static_cast<int>(fsm->GetHonmunType()));
    OutputDebugStringA(debugMsg);
}

void HonmunIdleState::Update(HonmunFSM* fsm)
{
    idleTimer += Time::GetDeltaTime();
    
    // 키네마틱 모드로 15초에 화면 위에서 바닥까지 이동
    auto transform = fsm->GetTransform();
    
    if (transform)
    {
        // 타입별 낙하 속도 적용
        float baseFallSpeed = 72.0f; // 기본 속도 (pixels/second)
        float fallSpeed = baseFallSpeed;
        float screenHeight = 1080.0f; // 화면 높이
        
        // 타입별 속도 배율 적용
        switch (fsm->GetHonmunType())
        {
        case HonmunType::A:
        case HonmunType::B:
        case HonmunType::C:
        case HonmunType::A2:
        case HonmunType::b:
            fallSpeed = baseFallSpeed * 1.0f; // 1배 속도
            break;
        case HonmunType::D:
            fallSpeed = baseFallSpeed * 2.0f; // 2배 속도
            break;
        default:
            fallSpeed = baseFallSpeed * 1.0f;
            break;
        }
        
        // 좌우 자연스러운 이동 (사인 파 패턴)
        float horizontalSpeed = 30.0f;
        float horizontalOffset = sinf(idleTimer * 2.0f) * horizontalSpeed * Time::GetDeltaTime();
        
        // 위치 업데이트 (Y축 방향 확인 - 아래로 가려면 Y값 감소)
        Vector2 currentPos = transform->GetPosition();
        Vector2 newPos = Vector2(
            currentPos.x + horizontalOffset,
            currentPos.y - fallSpeed * Time::GetDeltaTime()  // 아래로 가기 위해 - 사용
        );
        
        transform->SetPosition(newPos.x, newPos.y);
        
        // 화면 바닥에 도달하면 리셋 (Y가 화면 아래로 가면)
        if (newPos.y < -100.0f)  // Y값이 감소해서 -100 이하가 되면
        {
            // 다시 화면 위로 리셋
            transform->SetPosition(newPos.x, screenHeight + 100.0f);
        }
    }
    
    // 수동 애니메이션 시스템 (스프라이트 시트에서 프레임 변경)
    fsm->UpdateManualAnimation();
}

void HonmunIdleState::Exit(HonmunFSM* fsm)
{
    // Clean up when exiting idle state
    idleTimer = 0.0f;
}

// HonmunFSM Implementation
void HonmunFSM::OnEnable()
{
    // Component activation
}

void HonmunFSM::Awake()
{
    // Get component references
    honmun = dynamic_cast<Honmun*>(gameObject);
    animator = gameObject->GetComponent<Animator>();
    transform = gameObject->GetComponent<Transform>();
    rigidbody = gameObject->GetComponent<Rigidbody>();
    
    // Setup rigidbody for movement (Honmun.cpp에서 이미 설정하므로 추가 설정 불필요)
    
    // Get animator controller like PlayerFSM does
    if (animator != nullptr && animator->controller != nullptr)
    {
        animatorController = animator->controller;
    }
}

void HonmunFSM::Start()
{
    InitializeIdleState();
}

void HonmunFSM::Update()
{
    // Update the current state
    FSMBase<HonmunFSM, HonmunState>::Update();
}

void HonmunFSM::FixedUpdate()
{
    // Fixed update for physics-related state updates
    FSMBase<HonmunFSM, HonmunState>::FixedUpdate();
}

void HonmunFSM::OnDestroy()
{
    // Cleanup
}

void HonmunFSM::InitializeIdleState()
{
    // Start with idle state
    auto idleState = std::make_unique<HonmunIdleState>();
    ChangeState(std::move(idleState));
}

void HonmunFSM::PlayIdleAnimation()
{
    auto animatorController = GetAnimatorController();
    if (animatorController != nullptr)
    {
        const char* clipName = GetAnimationClipName();
        animatorController->PlayAnimation(clipName);
    }
}

const char* HonmunFSM::GetAnimationClipName() const
{
    switch (honmunType)
    {
    case HonmunType::A:
        return "Honmun_A_Idle";
    case HonmunType::B:
        return "Honmun_B_Idle";
    case HonmunType::C:
        return "Honmun_C_Idle";
    case HonmunType::D:
        return "Honmun_D_Idle";
    default:
        return "Honmun_A_Idle";
    }
}

void HonmunFSM::UpdateManualAnimation()
{
    if (!honmun) return;
    
    // 수동 애니메이션 타이머 업데이트
    animationTimer += Time::GetDeltaTime();
    
    // 프레임 간격에 도달하면 다음 프레임으로 이동
    if (animationTimer >= frameInterval)
    {
        animationTimer = 0.0f;
        currentFrame = (currentFrame + 1) % maxFrames;
        
        // 스프라이트 렌더러 가져오기
        auto* spriteRenderer = gameObject->GetComponent<SpriteRenderer>();
        if (spriteRenderer && spriteRenderer->sprite)
        {
            // 스프라이트 시트에서 현재 프레임 위치 계산 (128x128 크기)
            D2D1_RECT_F frameRect;
            frameRect.left = static_cast<float>(currentFrame * 128);    // 프레임 인덱스 * 프레임 폭
            frameRect.top = 0.0f;                                       // Y는 항상 0 (한 줄)
            frameRect.right = frameRect.left + 128.0f;                  // 프레임 폭
            frameRect.bottom = 128.0f;                                  // 프레임 높이
            
            // 피벗 포인트 (중앙)
            D2D1_POINT_2F pivotPoint = { 0.5f, 0.5f };
            
            // 현재 텍스처 가져오기
            auto texture = ResourceManager::Get().CreateTexture2D(honmun->GetTexturePath());
            if (texture)
            {
                // 새 스프라이트 이름 생성
                char spriteName[50];
                sprintf_s(spriteName, "Honmun_%c_%d", 
                         static_cast<char>('A' + static_cast<int>(honmunType)), currentFrame);
                
                // 새 스프라이트 생성 및 적용
                spriteRenderer->sprite = ResourceManager::Get().CreateSprite(texture, spriteName, frameRect, pivotPoint);
                
                // TEMP FIX: 애니메이션 로그 스팸 방지를 위해 비활성화
                // if (currentFrame == 0 || currentFrame == maxFrames - 1)
                // {
                //     char debugMsg[100];
                //     sprintf_s(debugMsg, "Type %d frame %d updated (%.1f,%.1f,%.1f,%.1f)\n", 
                //              static_cast<int>(honmunType), currentFrame, 
                //              frameRect.left, frameRect.top, frameRect.right, frameRect.bottom);
                //     OutputDebugStringA(debugMsg);
                // }
            }
        }
    }
}