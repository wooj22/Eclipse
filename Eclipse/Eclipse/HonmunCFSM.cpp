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
    
    // Start idle animation
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
    
    // Honmun C stays in idle state - no special state transitions needed
    // C type behavior is handled by collision system
}

void HonmunCIdleState::Exit(HonmunCFSM* fsm)
{
    // Clean up when exiting idle state
    idleTimer = 0.0f;
}

// HonmunCFSM Implementation
void HonmunCFSM::OnEnable()
{
    // Called when the script is enabled
}

void HonmunCFSM::Awake()
{
    // Get component references
    honmun = dynamic_cast<Honmun*>(gameObject);
    animator = gameObject->GetComponent<Animator>();
    transform = gameObject->GetComponent<Transform>();
    rigidbody = gameObject->GetComponent<Rigidbody>();
    
    // Get animator controller
    if (animator != nullptr && animator->controller != nullptr)
    {
        animatorController = animator->controller;
    }
}

void HonmunCFSM::Start()
{
    // Initialize with idle state
    InitializeIdleState();
}

void HonmunCFSM::Update()
{
    // Update the FSM
    FSMBase<HonmunCFSM, HonmunCState>::Update();
}

void HonmunCFSM::FixedUpdate()
{
    // Update the FSM fixed update
    FSMBase<HonmunCFSM, HonmunCState>::FixedUpdate();
}

void HonmunCFSM::OnDestroy()
{
    // Cleanup when script is destroyed
}

void HonmunCFSM::InitializeIdleState()
{
    // Start with idle state
    auto idleState = std::make_unique<HonmunCIdleState>();
    ChangeState(std::move(idleState));
}