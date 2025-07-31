#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include <iostream>
#include <exception>
#include <string>
using namespace std;

/*------------ Animation Clip ------------*/
class HonmunBIdleClip : public AnimationClip
{
public:
    HonmunBIdleClip()
    {
        name = "Honmun_B_Idle"; // 애니메이션 클립 이름 설정
        try
        {
            auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Aron/Honmun_b.png");
            if (texture != nullptr)
            {
                LoadSpriteSheetFromJson(texture, "../Resource/Aron/Data/SpriteSheet/Honmun_B_sprites.json");
                LoadAnimationClipFromJson(texture, "../Resource/Aron/Data/AnimationClip/Honmun_B_Idle_AniClip.json");
            }
        }
        catch (...)
        {
            // JSON 로딩 실패 시 무시
        }
    }
    ~HonmunBIdleClip() override {}
};

/*------------ Animation State ------------*/
class HonmunBIdleAnimState : public AnimationBaseState
{
public:
    HonmunBIdleAnimState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

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
class HonmunBAnimatorController : public AnimatorController
{
public:
    // Clips
    HonmunBIdleClip* idleClip = nullptr;

    // States
    HonmunBIdleAnimState* idleState = nullptr;

public: 
    HonmunBAnimatorController()
    {
        try
        {
            // Create clip
            idleClip = new HonmunBIdleClip();

            // Create state
            idleState = new HonmunBIdleAnimState(idleClip, this);

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
    
    ~HonmunBAnimatorController() override
    {
        delete idleClip;
        delete idleState;
    }
};