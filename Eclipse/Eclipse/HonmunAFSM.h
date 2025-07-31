#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "FSMBase.h"
#include "StateBase.h"

class Honmun;
class Animator;
class Transform;
class Rigidbody;

class HonmunAFSM;
class AnimatorController;

class HonmunAState : public StateBase<HonmunAFSM>
{
public:
    virtual ~HonmunAState() = default;
    virtual void Enter(HonmunAFSM* fsm) override = 0;
    virtual void Update(HonmunAFSM* fsm) override = 0;
    virtual void Exit(HonmunAFSM* fsm) override = 0;
    virtual void FixedUpdate(HonmunAFSM* fsm) {}
};

class HonmunAIdleState : public HonmunAState
{
private:
    float idleTimer = 0.0f;
    
public:
    void Enter(HonmunAFSM* fsm) override;
    void Update(HonmunAFSM* fsm) override;
    void Exit(HonmunAFSM* fsm) override;
};

class HonmunAFSM : public FSMBase<HonmunAFSM, HonmunAState>, public Script
{
private:
    Honmun* honmun = nullptr;
    Animator* animator = nullptr;
    AnimatorController* animatorController = nullptr;
    Transform* transform = nullptr;
    Rigidbody* rigidbody = nullptr;
    
public:
    HonmunAFSM() = default;
    virtual ~HonmunAFSM() = default;
    
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