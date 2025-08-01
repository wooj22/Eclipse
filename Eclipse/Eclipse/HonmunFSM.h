#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "FSMBase.h"
#include "StateBase.h"

class Honmun;
class Animator;
class Transform;
class Rigidbody;
class AnimatorController;

enum class HonmunType
{
    A = 0,  // Ignis
    B = 1,  // Umbra  
    C = 2,  // Darkness
    D = 3   // Luna
};

class HonmunFSM;

class HonmunState : public StateBase<HonmunFSM>
{
public:
    virtual ~HonmunState() = default;
    virtual void Enter(HonmunFSM* fsm) override = 0;
    virtual void Update(HonmunFSM* fsm) override = 0;
    virtual void Exit(HonmunFSM* fsm) override = 0;
    virtual void FixedUpdate(HonmunFSM* fsm) {}
};

class HonmunIdleState : public HonmunState
{
private:
    float idleTimer = 0.0f;
    
public:
    void Enter(HonmunFSM* fsm) override;
    void Update(HonmunFSM* fsm) override;
    void Exit(HonmunFSM* fsm) override;
};

class HonmunFSM : public FSMBase<HonmunFSM, HonmunState>, public Script
{
private:
    Honmun* honmun = nullptr;
    Animator* animator = nullptr;
    AnimatorController* animatorController = nullptr;
    Transform* transform = nullptr;
    Rigidbody* rigidbody = nullptr;
    
    HonmunType honmunType = HonmunType::A;
    
public:
    HonmunFSM() = default;
    virtual ~HonmunFSM() = default;
    
    // Getters for components
    Honmun* GetHonmun() const { return honmun; }
    Animator* GetAnimator() const { return animator; }
    AnimatorController* GetAnimatorController() const { return animatorController; }
    Transform* GetTransform() const { return transform; }
    Rigidbody* GetRigidbody() const { return rigidbody; }
    
    // Type management
    HonmunType GetHonmunType() const { return honmunType; }
    void SetHonmunType(HonmunType type) { honmunType = type; }
    
    // Script component lifecycle
    void OnEnable() override;
    void Awake() override;
    void Start() override;
    void Update() override;
    void FixedUpdate() override;
    void OnDestroy() override;
    
    // Initialize idle state
    void InitializeIdleState();
    
    // Animation helpers
    void PlayIdleAnimation();
    const char* GetAnimationClipName() const;
};