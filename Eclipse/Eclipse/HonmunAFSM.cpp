#include "HonmunAFSM.h"
#include "Honmun.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include <memory>

// HonmunAIdleState Implementation
void HonmunAIdleState::Enter(HonmunAFSM* fsm)
{
    idleTimer = 0.0f;
    
    // Start idle animation - use PlayerFSM approach
    auto animatorController = fsm->GetAnimatorController();
    if (animatorController != nullptr)
    {
        // Play the Honmun_A_Idle animation
        animatorController->PlayAnimation("Honmun_A_Idle");
    }
}

void HonmunAIdleState::Update(HonmunAFSM* fsm)
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

void HonmunAIdleState::Exit(HonmunAFSM* fsm)
{
    // Clean up when exiting idle state
    idleTimer = 0.0f;
}

// HonmunAFSM Implementation
void HonmunAFSM::OnEnable()
{
    // Component activation
}

void HonmunAFSM::Awake()
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

void HonmunAFSM::Start()
{
    InitializeIdleState();
}

void HonmunAFSM::Update()
{
    // Update the current state
    FSMBase<HonmunAFSM, HonmunAState>::Update();
}

void HonmunAFSM::FixedUpdate()
{
    // Fixed update for physics-related state updates
    FSMBase<HonmunAFSM, HonmunAState>::FixedUpdate();
}

void HonmunAFSM::OnDestroy()
{
    // Cleanup
}

void HonmunAFSM::InitializeIdleState()
{
    // Start with idle state
    auto idleState = std::make_unique<HonmunAIdleState>();
    ChangeState(std::move(idleState));
}