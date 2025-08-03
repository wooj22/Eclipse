#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "FSMBase.h"
#include "StateBase.h"

class Honmun;
class Animator;
class Transform;
class Rigidbody;

class HonmunBFSM;
class AnimatorController;

class HonmunBState : public StateBase<HonmunBFSM>
{
public:
    virtual ~HonmunBState() = default;
    virtual void Enter(HonmunBFSM* fsm) override = 0;
    virtual void Update(HonmunBFSM* fsm) override = 0;
    virtual void Exit(HonmunBFSM* fsm) override = 0;
    virtual void FixedUpdate(HonmunBFSM* fsm) {}
};

class HonmunBIdleState : public HonmunBState
{
private:
    float idleTimer = 0.0f;
    
public:
    void Enter(HonmunBFSM* fsm) override;
    void Update(HonmunBFSM* fsm) override;
    void Exit(HonmunBFSM* fsm) override;
};

class HonmunBFragmentState : public HonmunBState
{
private:
    float fragmentTimer = 0.0f;
    
public:
    void Enter(HonmunBFSM* fsm) override;
    void Update(HonmunBFSM* fsm) override;
    void Exit(HonmunBFSM* fsm) override;
};

class HonmunBFSM : public FSMBase<HonmunBFSM, HonmunBState>, public Script
{
private:
    Honmun* honmun = nullptr;
    Animator* animator = nullptr;
    AnimatorController* animatorController = nullptr;
    Transform* transform = nullptr;
    Rigidbody* rigidbody = nullptr;
    
public:
    HonmunBFSM() = default;
    virtual ~HonmunBFSM() = default;
    
    // Getters for components
    Honmun* GetHonmun() const { return honmun; }
    Animator* GetAnimator() const { return animator; }
    AnimatorController* GetAnimatorController() const { return animatorController; }
    Transform* GetTransform() const { return transform; }
    Rigidbody* GetRigidbody() const { return rigidbody; }
    
    // Script component lifecycle
    void OnEnable() override;
    void Awake() override;
    void Start() override;
    void Update() override;
    void FixedUpdate() override;
    void OnDestroy() override;
    
    // Initialize idle state
    void InitializeIdleState();
};