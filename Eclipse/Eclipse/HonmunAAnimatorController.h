#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include <iostream>
#include <exception>

/*------------ Animation Clip ------------*/
class HonmunAIdleClip : public AnimationClip
{
public:
    HonmunAIdleClip()
    {
        try
        {
            auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Aron/Honmun_a.png");
            if (texture != nullptr)
            {
                LoadSpriteSheetFromJson(texture, "../Resource/Aron/Data/SpriteSheet/Honmun_A_sprites.json");
                LoadAnimationClipFromJson(texture, "../Resource/Aron/Data/AnimationClip/Honmun_A_Idle_AniClip.json");
            }
        }
        catch (...)
        {
            // JSON 로딩 실패 시 무시
        }
    }
    ~HonmunAIdleClip() override {}
};

/*------------ Animation State ------------*/
class HonmunAIdleAnimState : public AnimationBaseState
{
public:
    HonmunAIdleAnimState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

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
class HonmunAAnimatorController : public AnimatorController
{
public:
    // Clips
    HonmunAIdleClip* idleClip = nullptr;

    // States
    HonmunAIdleAnimState* idleState = nullptr;

public: 
    HonmunAAnimatorController()
    {
        try
        {
            // Create clip
            idleClip = new HonmunAIdleClip();

            // Create state
            idleState = new HonmunAIdleAnimState(idleClip, this);

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
    
    ~HonmunAAnimatorController() override
    {
        delete idleClip;
        delete idleState;
    }
};