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



/*------------ Animation State ------------*/
class PlayerIdleState : public AnimationBaseState
{
public:
    PlayerIdleState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
    void Update(float dt) override
    {
        // [ tansition ]
        if (controller->GetBool("Samurai_Walk") == true)            controller->PlayAnimation("Samurai_Walk");
        // else if (controller->GetBool("Samurai_Walk") == true)    controller->PlayAnimation("Cat_Walk");
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
        // else if (controller->GetBool("Samurai_Walk") == true)    controller->PlayAnimation("Cat_Walk");
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

    // States
    PlayerIdleState* idleState = nullptr;
    PlayerWalkState* walkState = nullptr;

public: 
    PlayerAnimatorController()
    {
        // clip 생성
        idleClip = new PlayerIdleClip();
        walkClip = new PlayerWalkClip();

        // state 생성
        idleState = new PlayerIdleState(idleClip, this);
        walkState = new PlayerWalkState(walkClip, this);

        // state 등록
        AddState(idleState);
        AddState(walkState);
        // AddState(runState);

        // 초기 상태
        ChangeAnimation(idleState);
    }
    ~PlayerAnimatorController() override
    {
        delete idleClip;
        delete walkClip;
    }
};

