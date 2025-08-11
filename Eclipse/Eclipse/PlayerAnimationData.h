#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

// #include "PlayerAnimatorController.h"

// ∞‘¿” ƒ‹≈Ÿ√˜ Animation asset ¡§¿«
/*------------ Animation Clip ------------*/

// [ N ]
class PlayerIdleClip : public AnimationClip
{
public:
    PlayerIdleClip() // ≥π¿Â √‚∑¬ 
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
        name = "N_Player_Walk";
        loop = true;
        duration = 1.6f;

        const std::string basePath = "../Resource/Moon/Texture/N_Player_Walk/";
        const int frameCount = 20;
        const float frameTime = duration / frameCount;

        for (int i = 0; i < frameCount; ++i)
        {
            // "Walk (0).png", ...
            char filename[256];
            sprintf_s(filename, "%sN_Walk (%d).png", basePath.c_str(), i);

            auto texture = ResourceManager::Get().CreateTexture2D(filename);
            auto sprite = ResourceManager::Get().CreateSprite(texture, "N_Walk_" + std::to_string(i)); // ¿Ã∏ß¿∫ ±◊¥Î∑Œ µ÷µµ π´πÊ

            AnimationFrame frame;
            frame.sprite = sprite;
            frame.time = i * frameTime;

            frames.push_back(frame);
        }
    }
    ~PlayerWalkClip() override {}
};

class PlayerJumpClip : public AnimationClip
{
public:
    PlayerJumpClip()
    {
        name = "N_Player_Jump";
        loop = true;
        duration = 1.6f;

        const std::string basePath = "../Resource/Moon/Texture/N_Player_Jump/";
        const int frameCount = 20;
        const float frameTime = duration / frameCount;

        for (int i = 0; i < frameCount; ++i)
        {
            char filename[256];
            sprintf_s(filename, "%sN_Jump (%d).png", basePath.c_str(), i);

            auto texture = ResourceManager::Get().CreateTexture2D(filename);
            auto sprite = ResourceManager::Get().CreateSprite(texture, "N_Jump_" + std::to_string(i));

            AnimationFrame frame;
            frame.sprite = sprite;
            frame.time = i * frameTime;

            frames.push_back(frame);
        }
    }
    ~PlayerJumpClip() override {}
};

class PlayerHangingClip : public AnimationClip
{
public:
    PlayerHangingClip()
    {
        name = "N_Player_Hanging";
        loop = true;
        duration = 1.5f;

        const std::string basePath = "../Resource/Moon/Texture/N_Player_Hanging/";
        const int frameCount = 20;
        const float frameTime = duration / frameCount;

        for (int i = 0; i < frameCount; ++i)
        {
            char filename[256];
            sprintf_s(filename, "%sN_Hanging (%d).png", basePath.c_str(), i);

            auto texture = ResourceManager::Get().CreateTexture2D(filename);
            auto sprite = ResourceManager::Get().CreateSprite(texture, "N_Hanging_" + std::to_string(i));

            AnimationFrame frame;
            frame.sprite = sprite;
            frame.time = i * frameTime;

            frames.push_back(frame);
        }
    }
    ~PlayerHangingClip() override {}
};

class PlayerAttackClip : public AnimationClip
{
public:
    PlayerAttackClip()
    {
        name = "N_Player_Attack";
        loop = true;
        duration = 0.7f;

        const std::string basePath = "../Resource/Moon/Texture/N_Player_Attack/";
        const int frameCount = 20;
        const float frameTime = duration / frameCount;

        for (int i = 0; i < frameCount; ++i)
        {
            char filename[256];
            sprintf_s(filename, "%sN_Attack (%d).png", basePath.c_str(), i);

            auto texture = ResourceManager::Get().CreateTexture2D(filename);
            auto sprite = ResourceManager::Get().CreateSprite(texture, "N_Attack_" + std::to_string(i));

            AnimationFrame frame;
            frame.sprite = sprite;
            frame.time = i * frameTime;

            frames.push_back(frame);
        }
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
        name = "N_Player_Dash";
        loop = true;
        duration = 0.5f;

        const std::string basePath = "../Resource/Moon/Texture/N_Player_Dash/";
        const int frameCount = 20;
        const float frameTime = duration / frameCount;

        for (int i = 0; i < frameCount; ++i)
        {
            char filename[256];
            sprintf_s(filename, "%sN_Dash (%d).png", basePath.c_str(), i);

            auto texture = ResourceManager::Get().CreateTexture2D(filename);
            auto sprite = ResourceManager::Get().CreateSprite(texture, "N_Dash_" + std::to_string(i));

            AnimationFrame frame;
            frame.sprite = sprite;
            frame.time = i * frameTime;

            frames.push_back(frame);
        }
    }
    ~PlayerDashClip() override {}
};


// [ Y ]
class PlayerIdleClip_Y : public AnimationClip
{
public:
    PlayerIdleClip_Y() // ≥π¿Â √‚∑¬ 
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
        name = "Y_Player_Walk";
        loop = true;
        duration = 1.6f;

        const std::string basePath = "../Resource/Moon/Texture/Y_Player_Walk/";
        const int frameCount = 20;
        const float frameTime = duration / frameCount;

        for (int i = 0; i < frameCount; ++i)
        {
            char filename[256];
            sprintf_s(filename, "%sY_Walk (%d).png", basePath.c_str(), i);

            auto texture = ResourceManager::Get().CreateTexture2D(filename);
            auto sprite = ResourceManager::Get().CreateSprite(texture, "Y_Walk_" + std::to_string(i));

            AnimationFrame frame;
            frame.sprite = sprite;
            frame.time = i * frameTime;

            frames.push_back(frame);
        }
    }
    ~PlayerWalkClip_Y() override {}
};

class PlayerJumpClip_Y : public AnimationClip
{
public:
    PlayerJumpClip_Y()
    {
        name = "Y_Player_Jump";
        loop = true;
        duration = 1.6f;

        const std::string basePath = "../Resource/Moon/Texture/Y_Player_Jump/";
        const int frameCount = 20;
        const float frameTime = duration / frameCount;

        for (int i = 0; i < frameCount; ++i)
        {
            char filename[256];
            sprintf_s(filename, "%sY_Jump (%d).png", basePath.c_str(), i);

            auto texture = ResourceManager::Get().CreateTexture2D(filename);
            auto sprite = ResourceManager::Get().CreateSprite(texture, "Y_Jump_" + std::to_string(i));

            AnimationFrame frame;
            frame.sprite = sprite;
            frame.time = i * frameTime;

            frames.push_back(frame);
        }
    }
    ~PlayerJumpClip_Y() override {}
};

class PlayerHangingClip_Y : public AnimationClip
{
public:
    PlayerHangingClip_Y()
    {
        name = "Y_Player_Hanging";
        loop = true;
        duration = 1.5f;

        const std::string basePath = "../Resource/Moon/Texture/Y_Player_Hanging/";
        const int frameCount = 20;
        const float frameTime = duration / frameCount;

        for (int i = 0; i < frameCount; ++i)
        {
            char filename[256];
            sprintf_s(filename, "%sY_Hanging (%d).png", basePath.c_str(), i);

            auto texture = ResourceManager::Get().CreateTexture2D(filename);
            auto sprite = ResourceManager::Get().CreateSprite(texture, "Y_Hanging_" + std::to_string(i));

            AnimationFrame frame;
            frame.sprite = sprite;
            frame.time = i * frameTime;

            frames.push_back(frame);
        }
    }
    ~PlayerHangingClip_Y() override {}
};

class PlayerAttackClip_Y : public AnimationClip
{
public:
    PlayerAttackClip_Y()
    {
        name = "Y_Player_Attack";
        loop = true;
        duration = 1.0f;

        const std::string basePath = "../Resource/Moon/Texture/Y_Player_Attack/";
        const int frameCount = 20;
        const float frameTime = duration / frameCount;

        for (int i = 0; i < frameCount; ++i)
        {
            char filename[256];
            sprintf_s(filename, "%sY_Attack (%d).png", basePath.c_str(), i);

            auto texture = ResourceManager::Get().CreateTexture2D(filename);
            auto sprite = ResourceManager::Get().CreateSprite(texture, "Y_Attack_" + std::to_string(i));

            AnimationFrame frame;
            frame.sprite = sprite;
            frame.time = i * frameTime;

            frames.push_back(frame);
        }
    }
    ~PlayerAttackClip_Y() override {}
};

class PlayerDashClip_Y : public AnimationClip
{
public:
    PlayerDashClip_Y()
    {
        name = "Y_Player_Dash";
        loop = true;
        duration = 0.5f;

        const std::string basePath = "../Resource/Moon/Texture/Y_Player_Dash/";
        const int frameCount = 20;
        const float frameTime = duration / frameCount;

        for (int i = 0; i < frameCount; ++i)
        {
            char filename[256];
            sprintf_s(filename, "%sY_Dash (%d).png", basePath.c_str(), i);

            auto texture = ResourceManager::Get().CreateTexture2D(filename);
            auto sprite = ResourceManager::Get().CreateSprite(texture, "Y_Dash_" + std::to_string(i));

            AnimationFrame frame;
            frame.sprite = sprite;
            frame.time = i * frameTime;

            frames.push_back(frame);
        }
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
    void Update(float dt) override;
    void Exit() override {}
};

class PlayerWalkState : public AnimationBaseState
{
public:
    PlayerWalkState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override;
    void Exit() override {}
};

class PlayerJumpState : public AnimationBaseState
{
public:
    PlayerJumpState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override;
    void Exit() override {}
};

class PlayerHangingState : public AnimationBaseState
{
public:
    PlayerHangingState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override;
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
    void Update(float dt) override;
    void Exit() override {}
};

class PlayerAttackState : public AnimationBaseState
{
public:
    PlayerAttackState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override;
    void Exit() override {}
};



// [ Y ]
class PlayerIdleState_Y : public AnimationBaseState
{
public:
    PlayerIdleState_Y(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override;
    void Exit() override {}
};

class PlayerWalkState_Y : public AnimationBaseState
{
public:
    PlayerWalkState_Y(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override;
    void Exit() override {}
};

class PlayerJumpState_Y : public AnimationBaseState
{
public:
    PlayerJumpState_Y(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override;
    void Exit() override {}
};

class PlayerHangingState_Y : public AnimationBaseState
{
public:
    PlayerHangingState_Y(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override;
    void Exit() override {}
};

class PlayerDashState_Y : public AnimationBaseState
{
public:
    PlayerDashState_Y(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override;
    void Exit() override {}
};

class PlayerAttackState_Y : public AnimationBaseState
{
public:
    PlayerAttackState_Y(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override;
    void Exit() override {}
};
