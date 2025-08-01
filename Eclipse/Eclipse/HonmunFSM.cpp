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
    
    // Start idle animation based on type
    fsm->PlayIdleAnimation();
}

void HonmunIdleState::Update(HonmunFSM* fsm)
{
    idleTimer += Time::GetDeltaTime();
    
    // Keep playing idle animation (it loops automatically)
    auto animator = fsm->GetAnimator();
    if (animator != nullptr)
    {
        // The animation should loop automatically based on the JSON settings
        // No need to manually restart it
    }
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