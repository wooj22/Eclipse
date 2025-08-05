#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

/*------------ Animation Clip ------------*/
class HonAIdleClip : public AnimationClip
{
public:
	HonAIdleClip()
	{
		auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Hon/Texture/HonA.png");
		LoadSpriteSheetFromJson(texture, "../Resource/Woo/Hon/Data/SpriteSheet/HonA_sprites.json");
		LoadAnimationClipFromJson(texture, "../Resource/Woo/Hon/Data/AnimationClip/HonA_Idle_AniClip.json");
	}
};


/*------------ Animation State ------------*/

class HonAIdleState : public AnimationBaseState
{
public:
    HonAIdleState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

    void Enter() override {}
	void Update(float dt) override {}
    void Exit() override {}
};



/*---------- Animation Controller ---------*/
class HonAAnimatorController : public AnimatorController
{
public:
	HonAIdleClip* idleClip = nullptr;
	HonAIdleState* idleState = nullptr;

public:
	HonAAnimatorController()
	{
		idleClip = new HonAIdleClip();
		idleState = new HonAIdleState(idleClip, this);

		AddState(idleState);
		ChangeAnimation(idleState);
	}

	~HonAAnimatorController()
	{
		delete idleClip;
		delete idleState;
	}
};

