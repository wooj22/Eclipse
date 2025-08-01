#include "HonmunCFSM.h"
#include "Honmun.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include <memory>

// HonmunCIdleState Implementation
void HonmunCIdleState::Enter(HonmunCFSM* fsm)
{
    idleTimer = 0.0f;
    
    // Start idle animation - use PlayerFSM approach
    auto animatorController = fsm->GetAnimatorController();
    if (animatorController != nullptr)
    {
        // Play the Honmun_C_Idle animation
        animatorController->PlayAnimation("Honmun_C_Idle");
    }
}

void HonmunCIdleState::Update(HonmunCFSM* fsm)
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

void HonmunCIdleState::Exit(HonmunCFSM* fsm)
{
    // Clean up when exiting idle state
    idleTimer = 0.0f;
}

// HonmunCFSM Implementation
void HonmunCFSM::OnEnable()
{
    // Component activation
}

void HonmunCFSM::Awake()
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

void HonmunCFSM::Start()
{
    InitializeIdleState();
}

void HonmunCFSM::Update()
{
    // Update the current state
    FSMBase<HonmunCFSM, HonmunCState>::Update();
}

void HonmunCFSM::FixedUpdate()
{
    // Fixed update for physics-related state updates
    FSMBase<HonmunCFSM, HonmunCState>::FixedUpdate();
}

void HonmunCFSM::OnDestroy()
{
    // Cleanup
}

void HonmunCFSM::InitializeIdleState()
{
    // Start with idle state
    auto idleState = std::make_unique<HonmunCIdleState>();
    ChangeState(std::move(idleState));
}