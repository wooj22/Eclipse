#include "HonmunBFSM.h"
#include "Honmun.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include <memory>

// HonmunBIdleState Implementation
void HonmunBIdleState::Enter(HonmunBFSM* fsm)
{
    idleTimer = 0.0f;
    
    // Start idle animation
    auto animatorController = fsm->GetAnimatorController();
    if (animatorController != nullptr)
    {
        // Play the Honmun_B_Idle animation
        animatorController->PlayAnimation("Honmun_B_Idle");
    }
}

void HonmunBIdleState::Update(HonmunBFSM* fsm)
{
    idleTimer += Time::GetDeltaTime();
    
    // Check if B type should transition to fragment state
    // This happens when B collides with B and splits into fragments
    auto honmun = fsm->GetHonmun();
    if (honmun != nullptr && honmun->GetHP() == 1)
    {
        // Transition to fragment state when HP becomes 1
        auto fragmentState = std::make_unique<HonmunBFragmentState>();
        fsm->ChangeState(std::move(fragmentState));
    }
}

void HonmunBIdleState::Exit(HonmunBFSM* fsm)
{
    // Clean up when exiting idle state
    idleTimer = 0.0f;
}

// HonmunBFragmentState Implementation
void HonmunBFragmentState::Enter(HonmunBFSM* fsm)
{
    fragmentTimer = 0.0f;
    
    // Play the same idle animation but fragment behaves differently
    auto animatorController = fsm->GetAnimatorController();
    if (animatorController != nullptr)
    {
        animatorController->PlayAnimation("Honmun_B_Idle");
    }
    
    // Fragment-specific initialization
    // Fragments are smaller and have different physics properties
    auto honmun = fsm->GetHonmun();
    if (honmun != nullptr)
    {
        honmun->SetSize(0.7f); // Fragments are smaller
    }
}

void HonmunBFragmentState::Update(HonmunBFSM* fsm)
{
    fragmentTimer += Time::GetDeltaTime();
    
    // Fragments just exist until they collide with something
    // All collision logic is handled in HonmunCollisionScript
}

void HonmunBFragmentState::Exit(HonmunBFSM* fsm)
{
    fragmentTimer = 0.0f;
}

// HonmunBFSM Implementation
void HonmunBFSM::OnEnable()
{
    // Component activation
}

void HonmunBFSM::Awake()
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

void HonmunBFSM::Start()
{
    InitializeIdleState();
}

void HonmunBFSM::Update()
{
    // Update the current state
    FSMBase<HonmunBFSM, HonmunBState>::Update();
}

void HonmunBFSM::FixedUpdate()
{
    // Fixed update for physics-related state updates
    FSMBase<HonmunBFSM, HonmunBState>::FixedUpdate();
}

void HonmunBFSM::OnDestroy()
{
    // Cleanup
}

void HonmunBFSM::InitializeIdleState()
{
    // Start with idle state
    auto idleState = std::make_unique<HonmunBIdleState>();
    ChangeState(std::move(idleState));
}