#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include <iostream>
#include <exception>

/*------------ Animation Clip ------------*/
class HonmunDIdleClip : public AnimationClip
{
public:
    HonmunDIdleClip()
    {
        try
        {
            auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Aron/Honmun_d.png");
            if (texture != nullptr)
            {
                LoadSpriteSheetFromJson(texture, "../Resource/Aron/Data/SpriteSheet/Honmun_D_sprites.json");
                LoadAnimationClipFromJson(texture, "../Resource/Aron/Data/AnimationClip/Honmun_D_Idle_AniClip.json");
            }
            else
            {
                OutputDebugStringA("Failed to load Honmun_d.png texture\n");
            }
        }
        catch (const std::exception& e)
        {
            OutputDebugStringA("Honmun D animation loading failed\n");
        }
        catch (...)
        {
            OutputDebugStringA("Honmun D animation loading failed (unknown error)\n");
        }
    }
    ~HonmunDIdleClip() override {}
};

/*------------ Animation State ------------*/
class HonmunDIdleAnimState : public AnimationBaseState
{
public:
    HonmunDIdleAnimState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override 
    {
        // Start playing idle animation loop
    }
    
    void Update(float dt) override
    {
        // Idle animation loops automatically based on JSON settings
        // No state transitions for now - just stay in idle
    }
    
    void Exit() override 
    {
        // Clean up when leaving idle state
    }
};

/*------------ Animation Controller ------------*/
class HonmunDAnimatorController : public AnimatorController
{
public:
    // Clips
    HonmunDIdleClip* idleClip = nullptr;

    // States
    HonmunDIdleAnimState* idleState = nullptr;

public: 
    HonmunDAnimatorController()
    {
        try
        {
            // Create clip
            idleClip = new HonmunDIdleClip();

            // Create state
            idleState = new HonmunDIdleAnimState(idleClip, this);

            // Add state to controller
            AddState(idleState);

            // Start with idle animation
            ChangeAnimation(idleState);
        }
        catch (...)
        {
            // 초기화 실패 시 안전하게 처리
            if (idleClip) { delete idleClip; idleClip = nullptr; }
            if (idleState) { delete idleState; idleState = nullptr; }
        }
    }
    
    ~HonmunDAnimatorController() override
    {
        delete idleClip;
        delete idleState;
    }
};