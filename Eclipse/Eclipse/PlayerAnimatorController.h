#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"


// 게임 콘텐츠 Animation asset 정의
/*------------ Animation Clip ------------*/
class PlayerIdleClip : public AnimationClip
{
public:
    PlayerIdleClip()
    {
        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/Samurai_Idle.png");
        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/Samurai_Idle_Sprites.json");
        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/Samurai_Idle_AniClip.json");
    }
    ~PlayerIdleClip() override {}
};

class PlayerWalkClip : public AnimationClip
{
public:
    PlayerWalkClip()
    {
        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/Samurai_Walk.png");
        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/Samurai_Walk_Sprites.json");
        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/Samurai_Walk_AniClip.json");
    }
    ~PlayerWalkClip() override {}
};

class PlayerJumpClip : public AnimationClip
{
public:
    PlayerJumpClip()
    {
        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/Samurai_Jump.png");
        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/Samurai_Jump_Sprites.json");
        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/Samurai_Jump_AniClip.json");
    }
    ~PlayerJumpClip() override {}
};

class PlayerHangingClip : public AnimationClip
{
public:
    PlayerHangingClip()
    {
        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/Samurai_Hanging.png");
        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/Samurai_Hanging_Sprites.json");
        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/Samurai_Hanging_AniClip.json");
    }
    ~PlayerHangingClip() override {}
};

class PlayerAttackClip : public AnimationClip
{
public:
    PlayerAttackClip()
    {
        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/Samurai_Attack.png");
        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/Samurai_Attack_Sprites.json");
        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/Samurai_Attack_AniClip.json");
    }
    ~PlayerAttackClip() override {}
};

class PlayerFallClip : public AnimationClip
{
public:
    PlayerFallClip()
    {
        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/Samurai_Jump.png");
        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/Samurai_Jump_Sprites.json");
        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/Samurai_Jump_AniClip.json");
    }
    ~PlayerFallClip() override {}
};

//class PlayerDashClip : public AnimationClip
//{
//public:
//    PlayerDashClip()
//    {
//        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/Samurai_Dash.png");
//        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/Samurai_Dash_Sprites.json");
//        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/Samurai_Dash_AniClip.json");
//    }
//    ~PlayerDashClip() override {}
//};


/*------------ Animation State ------------*/

class PlayerIdleState : public AnimationBaseState
{
public:
    PlayerIdleState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override
    {
        // [ tansition ]
        if (controller->GetBool("Samurai_Walk") == true)         controller->PlayAnimation("Samurai_Walk");
        else if (controller->GetBool("Samurai_Jump") == true)    controller->PlayAnimation("Samurai_Jump");
        else if (controller->GetBool("Samurai_Dash") == true)    controller->PlayAnimation("Samurai_Dash");
        else if (controller->GetBool("Samurai_Hanging") == true) controller->PlayAnimation("Samurai_Hanging");
        else if (controller->GetBool("Samurai_Attack") == true)  controller->PlayAnimation("Samurai_Attack");
        else if (controller->GetBool("Samurai_Fall") == true)  controller->PlayAnimation("Samurai_Fall");
    }
    void Exit() override {}
};

class PlayerWalkState : public AnimationBaseState
{
public:
    PlayerWalkState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override
    {
        // [ tansition ]
        if (controller->GetBool("Samurai_Idle") == true)         controller->PlayAnimation("Samurai_Idle");
        else if (controller->GetBool("Samurai_Jump") == true)    controller->PlayAnimation("Samurai_Jump");
        else if (controller->GetBool("Samurai_Dash") == true)    controller->PlayAnimation("Samurai_Dash");
        else if (controller->GetBool("Samurai_Hanging") == true) controller->PlayAnimation("Samurai_Hanging");
        else if (controller->GetBool("Samurai_Attack") == true)  controller->PlayAnimation("Samurai_Attack");
        else if (controller->GetBool("Samurai_Fall") == true)  controller->PlayAnimation("Samurai_Fall");
    }
    void Exit() override {}
};

class PlayerJumpState : public AnimationBaseState
{
public:
    PlayerJumpState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override
    {
        // [ tansition ]
        if (controller->GetBool("Samurai_Idle") == true)         controller->PlayAnimation("Samurai_Idle");
        else if (controller->GetBool("Samurai_Walk") == true)    controller->PlayAnimation("Samurai_Walk");
        else if (controller->GetBool("Samurai_Dash") == true)    controller->PlayAnimation("Samurai_Dash");
        else if (controller->GetBool("Samurai_Hanging") == true) controller->PlayAnimation("Samurai_Hanging");
        else if (controller->GetBool("Samurai_Attack") == true)  controller->PlayAnimation("Samurai_Attack");
        else if (controller->GetBool("Samurai_Fall") == true)  controller->PlayAnimation("Samurai_Fall");
    }
    void Exit() override {}
};

class PlayerHangingState : public AnimationBaseState
{
public:
    PlayerHangingState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override
    {
        // [ tansition ]
        if (controller->GetBool("Samurai_Idle") == true)         controller->PlayAnimation("Samurai_Idle");
        else if (controller->GetBool("Samurai_Jump") == true)    controller->PlayAnimation("Samurai_Jump");
        else if (controller->GetBool("Samurai_Dash") == true)    controller->PlayAnimation("Samurai_Dash");
        else if (controller->GetBool("Samurai_Walk") == true)    controller->PlayAnimation("Samurai_Walk");
        else if (controller->GetBool("Samurai_Attack") == true)  controller->PlayAnimation("Samurai_Attack");
        else if (controller->GetBool("Samurai_Fall") == true)  controller->PlayAnimation("Samurai_Fall");
    }
    void Exit() override {}
};

class PlayerFallState : public AnimationBaseState
{
public:
    PlayerFallState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override
    {
        // [ tansition ]
        if (controller->GetBool("Samurai_Idle") == true)         controller->PlayAnimation("Samurai_Idle");
        else if (controller->GetBool("Samurai_Jump") == true)    controller->PlayAnimation("Samurai_Jump");
        else if (controller->GetBool("Samurai_Dash") == true)    controller->PlayAnimation("Samurai_Dash");
        else if (controller->GetBool("Samurai_Walk") == true)    controller->PlayAnimation("Samurai_Walk");
        else if (controller->GetBool("Samurai_Attack") == true)  controller->PlayAnimation("Samurai_Attack");
        else if (controller->GetBool("Samurai_Fall") == true)  controller->PlayAnimation("Samurai_Fall");
    }
    void Exit() override {}
};

//class PlayerDashState : public AnimationBaseState
//{
//public:
//    PlayerDashState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}
//
//    void Enter() override {}
//    void Update(float dt) override
//    {
//        // [ tansition ]
//        if (controller->GetBool("Samurai_Idle") == true)         controller->PlayAnimation("Samurai_Idle");
//        else if (controller->GetBool("Samurai_Jump") == true)    controller->PlayAnimation("Samurai_Jump");
//        else if (controller->GetBool("Samurai_Dash") == true)    controller->PlayAnimation("Samurai_Dash");
//        else if (controller->GetBool("Samurai_Walk") == true)    controller->PlayAnimation("Samurai_Walk");
//        else if (controller->GetBool("Samurai_Hanging") == true)         controller->PlayAnimation("Samurai_Hanging");
//        else if (controller->GetBool("Samurai_Attack") == true)    controller->PlayAnimation("Samurai_Attack");
//    }
//    void Exit() override {}
//};

class PlayerAttackState : public AnimationBaseState
{
public:
    PlayerAttackState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override
    {
        // [ tansition ]
        if (controller->GetBool("Samurai_Idle") == true)         controller->PlayAnimation("Samurai_Idle");
        else if (controller->GetBool("Samurai_Jump") == true)    controller->PlayAnimation("Samurai_Jump");
        else if (controller->GetBool("Samurai_Dash") == true)    controller->PlayAnimation("Samurai_Dash");
        else if (controller->GetBool("Samurai_Walk") == true)    controller->PlayAnimation("Samurai_Walk");
        else if (controller->GetBool("Samurai_Hanging") == true) controller->PlayAnimation("Samurai_Hanging");
        else if (controller->GetBool("Samurai_Fall") == true)  controller->PlayAnimation("Samurai_Fall");
    }
    void Exit() override {}
};



/*------------ Animation Controller ------------*/
class PlayerAnimatorController : public AnimatorController
{
public:
    // Clips
    PlayerIdleClip* idleClip = nullptr;
    PlayerWalkClip* walkClip = nullptr;
    PlayerJumpClip* jumpClip = nullptr;
    PlayerHangingClip* hangingClip = nullptr;
    PlayerAttackClip* attackClip = nullptr;
    PlayerFallClip* fallClip = nullptr;
    // PlayerDashClip* dashClip = nullptr;

    // States
    PlayerIdleState* idleState = nullptr;
    PlayerWalkState* walkState = nullptr;
    PlayerJumpState* jumpState = nullptr;
    PlayerHangingState* hangingState = nullptr;
    PlayerAttackState* attackState = nullptr;
    PlayerFallState* fallState = nullptr;
    // PlayerDashState* dashState = nullptr;

public: 
    PlayerAnimatorController()
    {
        // clip 생성
        idleClip = new PlayerIdleClip();
        walkClip = new PlayerWalkClip();
        jumpClip = new PlayerJumpClip();
        hangingClip = new PlayerHangingClip();
        attackClip = new PlayerAttackClip();
        fallClip = new PlayerFallClip();
        // dashClip = new PlayerDashClip();


        // state 생성
        idleState = new PlayerIdleState(idleClip, this);
        walkState = new PlayerWalkState(walkClip, this);
        jumpState = new PlayerJumpState(jumpClip, this);
        hangingState = new PlayerHangingState(hangingClip, this);
        attackState = new PlayerAttackState(attackClip, this);
        fallState = new PlayerFallState(fallClip, this);
        // dashState = new PlayerDashState(dashClip, this);

        // state 등록
        AddState(idleState);
        AddState(walkState);
        AddState(jumpState);
        AddState(hangingState);
        AddState(attackState);
        AddState(fallState);
        // AddState(dashState);

        // 초기 상태
        ChangeAnimation(idleState);
    }
    ~PlayerAnimatorController() override
    {
        delete idleClip;
        delete walkClip;
        delete jumpState;
        delete hangingState;
        delete attackState;
        delete fallState;
        // delete dashState;
    }
};
