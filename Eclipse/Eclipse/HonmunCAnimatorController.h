#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include <iostream>
#include <exception>

/*------------ Animation Clip ------------*/
class HonmunCIdleClip : public AnimationClip
{
public:
    HonmunCIdleClip()
    {
        try
        {
            auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Aron/Honmun_c.png");
            if (texture != nullptr)
            {
                LoadSpriteSheetFromJson(texture, "../Resource/Aron/Data/SpriteSheet/Honmun_C_sprites.json");
                LoadAnimationClipFromJson(texture, "../Resource/Aron/Data/AnimationClip/Honmun_C_Idle_AniClip.json");
            }
        }
        catch (...)
        {
            // JSON 로딩 실패 시 무시
        }
    }
    ~HonmunCIdleClip() override {}
};

/*------------ Animation State ------------*/
class HonmunCIdleAnimState : public AnimationBaseState
{
public:
    HonmunCIdleAnimState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

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
class HonmunCAnimatorController : public AnimatorController
{
public:
    // Clips
    HonmunCIdleClip* idleClip = nullptr;

    // States
    HonmunCIdleAnimState* idleState = nullptr;

public: 
    HonmunCAnimatorController()
    {
        try
        {
            // Create clip
            idleClip = new HonmunCIdleClip();

            // Create state
            idleState = new HonmunCIdleAnimState(idleClip, this);

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
    
    ~HonmunCAnimatorController() override
    {
        delete idleClip;
        delete idleState;
    }
};