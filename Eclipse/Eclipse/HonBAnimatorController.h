#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

/*------------ Animation Clip ------------*/
class HonBIdleClip : public AnimationClip
{
public:
	HonBIdleClip()
	{
		auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Hon/Texture/HonB.png");
		LoadSpriteSheetFromJson(texture, "../Resource/Woo/Hon/Data/SpriteSheet/HonB_sprites.json");
		LoadAnimationClipFromJson(texture, "../Resource/Woo/Hon/Data/AnimationClip/HonB_Idle_AniClip.json");
	}
};





/*------------ Animation State ------------*/
class HonBIdleState : public AnimationBaseState
{
public:
	HonBIdleState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

	void Enter() override {}
	void Update(float dt) override {}
	void Exit() override {}
};




/*---------- Animation Controller ---------*/
class HonBAnimatorController : public AnimatorController
{
public:
	HonBIdleClip* idleClip = nullptr;
	HonBIdleState* idleState = nullptr;

public:
	HonBAnimatorController()
	{
		idleClip = new HonBIdleClip();
		idleState = new HonBIdleState(idleClip, this);

		AddState(idleState);
		ChangeAnimation(idleState);
	}

	~HonBAnimatorController()
	{
		delete idleClip;
		delete idleState;
	}
};

