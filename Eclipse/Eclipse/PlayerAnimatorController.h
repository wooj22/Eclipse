#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"


// 게임 콘텐츠 Animation asset 정의
/*------------ Animation Clip ------------*/ 

// [ N ]
class PlayerIdleClip : public AnimationClip
{
public:
    PlayerIdleClip() // 낱장 출력 
    {
        name = "N_Player_Idle";
        loop = true;
        duration = 1.3f;

        const std::string basePath = "../Resource/Moon/Texture/N_Player_Idle/";
        const int frameCount = 20;
        const float frameTime = duration / frameCount;

        for (int i = 0; i < frameCount; ++i)
        {
            // Idle_00.png, Idle_01.png, ...
            char filename[256];
            sprintf_s(filename, "%sIdle_%02d.png", basePath.c_str(), i);

            auto texture = ResourceManager::Get().CreateTexture2D(filename);
            auto sprite = ResourceManager::Get().CreateSprite(texture, "Idle_" + std::to_string(i));

            AnimationFrame frame;
            frame.sprite = sprite;
            frame.time = i * frameTime;

            frames.push_back(frame);
        }
    }
    ~PlayerIdleClip() override {}
};

class PlayerWalkClip : public AnimationClip
{
public:
    PlayerWalkClip()
    {
        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/N_Player_Walk.png");
        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/N_Player_Walk_Sprites.json");
        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/N_Player_Walk_AniClip.json");
    }
    ~PlayerWalkClip() override {}
};

class PlayerJumpClip : public AnimationClip
{
public:
    PlayerJumpClip()
    {
        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/N_Player_Jump.png");
        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/N_Player_Jump_Sprites.json");
        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/N_Player_Jump_AniClip.json");
    }
    ~PlayerJumpClip() override {}
};

class PlayerHangingClip : public AnimationClip
{
public:
    PlayerHangingClip()
    {
        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/N_Player_Jump.png");
        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/N_Player_Jump_Sprites.json");
        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/N_Player_Jump_AniClip.json");
    }
    ~PlayerHangingClip() override {}
};

class PlayerAttackClip : public AnimationClip
{
public:
    PlayerAttackClip()
    {
        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/N_Player_Attack.png");
        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/N_Player_Attack_Sprites.json");
        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/N_Player_Attack_AniClip.json");
    }
    ~PlayerAttackClip() override {}
};

//class PlayerFallClip : public AnimationClip
//{
//public:
//    PlayerFallClip()
//    {
//        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/Samurai_Jump.png");
//        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/Samurai_Jump_Sprites.json");
//        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/Samurai_Jump_AniClip.json");
//    }
//    ~PlayerFallClip() override {}
//};

class PlayerDashClip : public AnimationClip
{
public:
    PlayerDashClip()
    {
        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/N_Player_Dash.png");
        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/N_Player_Dash_Sprites.json");
        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/N_Player_Dash_AniClip.json");
    }
    ~PlayerDashClip() override {}
};


// [ Y ]
class PlayerIdleClip_Y : public AnimationClip
{
public:
    PlayerIdleClip_Y() // 낱장 출력 
    {
        name = "Y_Player_Idle";
        loop = true;
        duration = 1.3f;

        const std::string basePath = "../Resource/Moon/Texture/Y_Player_Idle/";
        const int frameCount = 20;
        const float frameTime = duration / frameCount;

        for (int i = 0; i < frameCount; ++i)
        {
            char filename[256];
            sprintf_s(filename, "%sY_Idle_%02d.png", basePath.c_str(), i);

            auto texture = ResourceManager::Get().CreateTexture2D(filename);
            auto sprite = ResourceManager::Get().CreateSprite(texture, "Y_Idle_" + std::to_string(i));

            AnimationFrame frame;
            frame.sprite = sprite;
            frame.time = i * frameTime;

            frames.push_back(frame);
        }
    }
    ~PlayerIdleClip_Y() override {}
};

class PlayerWalkClip_Y : public AnimationClip
{
public:
    PlayerWalkClip_Y()
    {
        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/Y_Player_Walk.png");
        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/Y_Player_Walk_Sprites.json");
        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/Y_Player_Walk_AniClip.json");
    }
    ~PlayerWalkClip_Y() override {}
};

class PlayerJumpClip_Y : public AnimationClip
{
public:
    PlayerJumpClip_Y()
    {
        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/Y_Player_Jump.png");
        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/Y_Player_Jump_Sprites.json");
        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/Y_Player_Jump_AniClip.json");
    }
    ~PlayerJumpClip_Y() override {}
};

class PlayerHangingClip_Y : public AnimationClip
{
public:
    PlayerHangingClip_Y()
    {
        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/Y_Player_Jump.png");
        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/Y_Player_Jump_Sprites.json");
        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/Y_Player_Jump_AniClip.json");
    }
    ~PlayerHangingClip_Y() override {}
};

class PlayerAttackClip_Y : public AnimationClip
{
public:
    PlayerAttackClip_Y()
    {
        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/Y_Player_Attack.png");
        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/Y_Player_Attack_Sprites.json");
        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/Y_Player_Attack_AniClip.json");
    }
    ~PlayerAttackClip_Y() override {}
};

class PlayerDashClip_Y : public AnimationClip
{
public:
    PlayerDashClip_Y()
    {
        auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Texture/Y_Player_Dash.png");
        LoadSpriteSheetFromJson(texture, "../Resource/Moon/Data/SpriteSheet/Y_Player_Dash_Sprites.json");
        LoadAnimationClipFromJson(texture, "../Resource/Moon/Data/AnimationClip/Y_Player_Dash_AniClip.json");
    }
    ~PlayerDashClip_Y() override {}
};




/*------------ Animation State ------------*/

// [ N ]
class PlayerIdleState : public AnimationBaseState
{
public:
    PlayerIdleState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override
    {
        // [ tansition ]
        if (controller->GetBool("N_Player_Walk") == true)         controller->PlayAnimation("N_Player_Walk");
        else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
        else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
        else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");
        else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");

        else if (controller->GetBool("Y_Player_Idle") == true)    controller->PlayAnimation("Y_Player_Idle");
        else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
        else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
        else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
        else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");
        else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");
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
        if (controller->GetBool("N_Player_Idle") == true)         controller->PlayAnimation("N_Player_Idle");
        else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
        else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
        else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");
        else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");

        else if (controller->GetBool("Y_Player_Idle") == true)    controller->PlayAnimation("Y_Player_Idle");
        else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
        else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
        else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
        else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");
        else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");
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
        if (controller->GetBool("N_Player_Idle") == true)         controller->PlayAnimation("N_Player_Idle");
        else if (controller->GetBool("N_Player_Walk") == true)    controller->PlayAnimation("N_Player_Walk");
        else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
        else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");
        else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");
        // else if (controller->GetBool("N_Player_Fall") == true)  controller->PlayAnimation("N_Player_Jump");

        else if (controller->GetBool("Y_Player_Idle") == true)         controller->PlayAnimation("Y_Player_Idle");
        else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
        else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
        else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");
        else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");
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
        if (controller->GetBool("N_Player_Idle") == true)         controller->PlayAnimation("N_Player_Idle");
        else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
        else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
        else if (controller->GetBool("N_Player_Walk") == true)    controller->PlayAnimation("N_Player_Walk");
        else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");
        // else if (controller->GetBool("N_Player_Fall") == true)  controller->PlayAnimation("N_Player_Jump");

        else if (controller->GetBool("Y_Player_Idle") == true)         controller->PlayAnimation("Y_Player_Idle");
        else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
        else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
        else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
        else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");
    }
    void Exit() override {}
};

//class PlayerFallState : public AnimationBaseState
//{
//public:
//    PlayerFallState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}
//
//    void Enter() override {}
//    void Update(float dt) override
//    {
//        // [ tansition ]
//        if (controller->GetBool("Samurai_Idle") == true)         controller->PlayAnimation("Samurai_Idle");
//        else if (controller->GetBool("Samurai_Jump") == true)    controller->PlayAnimation("Samurai_Jump");
//        else if (controller->GetBool("Samurai_Dash") == true)    controller->PlayAnimation("Samurai_Dash");
//        else if (controller->GetBool("Samurai_Walk") == true)    controller->PlayAnimation("Samurai_Walk");
//        else if (controller->GetBool("Samurai_Attack") == true)  controller->PlayAnimation("Samurai_Attack");
//        // else if (controller->GetBool("Samurai_Fall") == true)  controller->PlayAnimation("Samurai_Jump");
//    }
//    void Exit() override {}
//};

class PlayerDashState : public AnimationBaseState
{
public:
    PlayerDashState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override
    {
        // [ tansition ]
        if (controller->GetBool("N_Player_Idle") == true)         controller->PlayAnimation("N_Player_Idle");
        else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
        else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
        else if (controller->GetBool("N_Player_Walk") == true)    controller->PlayAnimation("N_Player_Walk");
        else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");
        else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");

        else if (controller->GetBool("Y_Player_Idle") == true)         controller->PlayAnimation("Y_Player_Idle");
        else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
        else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
        else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
        else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");
        else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");
    }
    void Exit() override {}
};

class PlayerAttackState : public AnimationBaseState
{
public:
    PlayerAttackState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override
    {
        // [ tansition ]
        if (controller->GetBool("N_Player_Idle") == true)         controller->PlayAnimation("N_Player_Idle");
        else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
        else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
        else if (controller->GetBool("N_Player_Walk") == true)    controller->PlayAnimation("N_Player_Walk");
        else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");

        else if (controller->GetBool("Y_Player_Idle") == true)         controller->PlayAnimation("Y_Player_Idle");
        else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
        else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
        else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
        else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");
    }
    void Exit() override {}
};



// [ Y ]
class PlayerIdleState_Y : public AnimationBaseState
{
public:
    PlayerIdleState_Y(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override
    {
        // [ tansition ]
        if (controller->GetBool("Y_Player_Walk") == true)         controller->PlayAnimation("Y_Player_Walk");
        else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
        else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
        else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");
        else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");

        else if (controller->GetBool("N_Player_Idle") == true)         controller->PlayAnimation("N_Player_Idle");
        else if (controller->GetBool("N_Player_Walk") == true)         controller->PlayAnimation("N_Player_Walk");
        else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
        else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
        else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");
        else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");
    }
    void Exit() override {}
};

class PlayerWalkState_Y : public AnimationBaseState
{
public:
    PlayerWalkState_Y(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override
    {
        // [ tansition ]
        if (controller->GetBool("Y_Player_Idle") == true)         controller->PlayAnimation("Y_Player_Idle");
        else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
        else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
        else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");
        else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");

        else if (controller->GetBool("N_Player_Idle") == true)    controller->PlayAnimation("N_Player_Idle");
        else if (controller->GetBool("N_Player_Walk") == true)    controller->PlayAnimation("N_Player_Walk");
        else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
        else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
        else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");
        else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");
    }
    void Exit() override {}
};

class PlayerJumpState_Y : public AnimationBaseState
{
public:
    PlayerJumpState_Y(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override
    {
        // [ tansition ]
        if (controller->GetBool("Y_Player_Idle") == true)         controller->PlayAnimation("Y_Player_Idle");
        else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
        else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
        else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");
        else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");

        else if (controller->GetBool("N_Player_Idle") == true)    controller->PlayAnimation("N_Player_Idle");
        else if (controller->GetBool("N_Player_Walk") == true)    controller->PlayAnimation("N_Player_Walk");
        else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
        else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");
        else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");
        // else if (controller->GetBool("N_Player_Fall") == true)  controller->PlayAnimation("N_Player_Jump");
    }
    void Exit() override {}
};

class PlayerHangingState_Y : public AnimationBaseState
{
public:
    PlayerHangingState_Y(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override
    {
        // [ tansition ]
        if (controller->GetBool("Y_Player_Idle") == true)         controller->PlayAnimation("Y_Player_Idle");
        else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
        else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
        else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
        else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");

        else if (controller->GetBool("N_Player_Idle") == true)    controller->PlayAnimation("N_Player_Idle");
        else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
        else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
        else if (controller->GetBool("N_Player_Walk") == true)    controller->PlayAnimation("N_Player_Walk");
        else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");
        // else if (controller->GetBool("N_Player_Fall") == true)  controller->PlayAnimation("N_Player_Jump");
    }
    void Exit() override {}
};

class PlayerDashState_Y : public AnimationBaseState
{
public:
    PlayerDashState_Y(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override
    {
        // [ tansition ]
        if (controller->GetBool("Y_Player_Idle") == true)         controller->PlayAnimation("Y_Player_Idle");
        else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
        else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
        else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
        else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");
        else if (controller->GetBool("Y_Player_Attack") == true)  controller->PlayAnimation("Y_Player_Attack");

        else if (controller->GetBool("N_Player_Idle") == true)    controller->PlayAnimation("N_Player_Idle");
        else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
        else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
        else if (controller->GetBool("N_Player_Walk") == true)    controller->PlayAnimation("N_Player_Walk");
        else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");
        else if (controller->GetBool("N_Player_Attack") == true)  controller->PlayAnimation("N_Player_Attack");
    }
    void Exit() override {}
};

class PlayerAttackState_Y : public AnimationBaseState
{
public:
    PlayerAttackState_Y(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override
    {
        // [ tansition ]
        if (controller->GetBool("Y_Player_Idle") == true)         controller->PlayAnimation("Y_Player_Idle");
        else if (controller->GetBool("Y_Player_Jump") == true)    controller->PlayAnimation("Y_Player_Jump");
        else if (controller->GetBool("Y_Player_Dash") == true)    controller->PlayAnimation("Y_Player_Dash");
        else if (controller->GetBool("Y_Player_Walk") == true)    controller->PlayAnimation("Y_Player_Walk");
        else if (controller->GetBool("Y_Player_Hanging") == true) controller->PlayAnimation("Y_Player_Hanging");

        else if (controller->GetBool("N_Player_Idle") == true)    controller->PlayAnimation("N_Player_Idle");
        else if (controller->GetBool("N_Player_Jump") == true)    controller->PlayAnimation("N_Player_Jump");
        else if (controller->GetBool("N_Player_Dash") == true)    controller->PlayAnimation("N_Player_Dash");
        else if (controller->GetBool("N_Player_Walk") == true)    controller->PlayAnimation("N_Player_Walk");
        else if (controller->GetBool("N_Player_Hanging") == true) controller->PlayAnimation("N_Player_Hanging");
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
    // PlayerFallClip* fallClip = nullptr;
    PlayerDashClip* dashClip = nullptr;

    PlayerIdleClip_Y* idleClip_Y = nullptr;
    PlayerWalkClip_Y* walkClip_Y = nullptr;
    PlayerJumpClip_Y* jumpClip_Y = nullptr;
    PlayerHangingClip_Y* hangingClip_Y = nullptr;
    PlayerAttackClip_Y* attackClip_Y = nullptr;
    PlayerDashClip_Y* dashClip_Y = nullptr;

    // States
    PlayerIdleState* idleState = nullptr;
    PlayerWalkState* walkState = nullptr;
    PlayerJumpState* jumpState = nullptr;
    PlayerHangingState* hangingState = nullptr;
    PlayerAttackState* attackState = nullptr;
    // PlayerFallState* fallState = nullptr;
    PlayerDashState* dashState = nullptr;

    PlayerIdleState_Y* idleState_Y = nullptr;
    PlayerWalkState_Y* walkState_Y = nullptr;
    PlayerJumpState_Y* jumpState_Y = nullptr;
    PlayerHangingState_Y* hangingState_Y = nullptr;
    PlayerAttackState_Y* attackState_Y = nullptr;
    // PlayerFallState_Y* fallState_Y = nullptr;
    PlayerDashState_Y* dashState_Y = nullptr;

public: 
    PlayerAnimatorController()
    {
        // [ clip 생성 ]
        idleClip = new PlayerIdleClip();
        walkClip = new PlayerWalkClip();
        jumpClip = new PlayerJumpClip();
        hangingClip = new PlayerHangingClip();
        attackClip = new PlayerAttackClip();
        // fallClip = new PlayerFallClip();
        dashClip = new PlayerDashClip();

        idleClip_Y = new PlayerIdleClip_Y();
        walkClip_Y = new PlayerWalkClip_Y();
        jumpClip_Y = new PlayerJumpClip_Y();
        hangingClip_Y = new PlayerHangingClip_Y();
        attackClip_Y = new PlayerAttackClip_Y();
        dashClip_Y = new PlayerDashClip_Y();



        // [ state 생성 ]

        idleState = new PlayerIdleState(idleClip, this);
        walkState = new PlayerWalkState(walkClip, this);
        jumpState = new PlayerJumpState(jumpClip, this);
        hangingState = new PlayerHangingState(hangingClip, this);
        attackState = new PlayerAttackState(attackClip, this);
        // fallState = new PlayerFallState(fallClip, this);
        dashState = new PlayerDashState(dashClip, this);

        idleState_Y = new PlayerIdleState_Y(idleClip_Y, this);
        walkState_Y = new PlayerWalkState_Y(walkClip_Y, this);
        jumpState_Y = new PlayerJumpState_Y(jumpClip_Y, this);
        hangingState_Y = new PlayerHangingState_Y(hangingClip_Y, this);
        attackState_Y = new PlayerAttackState_Y(attackClip_Y, this);
        dashState_Y = new PlayerDashState_Y(dashClip_Y, this);



        // [ state 등록 ]

        AddState(idleState);
        AddState(walkState);
        AddState(jumpState);
        AddState(hangingState);
        AddState(attackState);
        // AddState(fallState);
        AddState(dashState);

        AddState(idleState_Y);
        AddState(walkState_Y);
        AddState(jumpState_Y);
        AddState(hangingState_Y);
        AddState(attackState_Y);
        AddState(dashState_Y);


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
        // delete fallState;
        delete dashState;

        delete idleClip_Y;
        delete walkClip_Y;
        delete jumpState_Y;
        delete hangingState_Y;
        delete attackState_Y;
        delete dashState_Y;
    }
};
