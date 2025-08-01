#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "FSMBase.h"
#include "StateBase.h"

class Honmun;
class Animator;
class Transform;
class Rigidbody;

class HonmunCFSM;
class AnimatorController;

class HonmunCState : public StateBase<HonmunCFSM>
{
public:
    virtual ~HonmunCState() = default;
    virtual void Enter(HonmunCFSM* fsm) override = 0;
    virtual void Update(HonmunCFSM* fsm) override = 0;
    virtual void Exit(HonmunCFSM* fsm) override = 0;
    virtual void FixedUpdate(HonmunCFSM* fsm) {}
};

class HonmunCIdleState : public HonmunCState
{
private:
    float idleTimer = 0.0f;
    
public:
    void Enter(HonmunCFSM* fsm) override;
    void Update(HonmunCFSM* fsm) override;
    void Exit(HonmunCFSM* fsm) override;
};

class HonmunCFSM : public FSMBase<HonmunCFSM, HonmunCState>, public Script
{
private:
    Honmun* honmun = nullptr;
    Animator* animator = nullptr;
    AnimatorController* animatorController = nullptr;
    Transform* transform = nullptr;
    Rigidbody* rigidbody = nullptr;
    
public:
    HonmunCFSM() = default;
    virtual ~HonmunCFSM() = default;
    
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