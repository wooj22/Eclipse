#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

/*------------ Animation Clip ------------*/
class HonDIdleClip : public AnimationClip
{
public:
	HonDIdleClip()
	{
		auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Hon/Texture/HonD.png");
		LoadSpriteSheetFromJson(texture, "../Resource/Woo/Hon/Data/SpriteSheet/HonD_sprites.json");
		LoadAnimationClipFromJson(texture, "../Resource/Woo/Hon/Data/AnimationClip/HonD_Idle_AniClip.json");
	}
};




/*------------ Animation State ------------*/
class HonDIdleState : public AnimationBaseState
{
public:
	HonDIdleState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

	void Enter() override {}
	void Update(float dt) override {}
	void Exit() override {}
};






/*---------- Animation Controller ---------*/
class HonDAnimatorController : public AnimatorController
{
public:
	HonDIdleClip* idleClip = nullptr;
	HonDIdleState* idleState = nullptr;

public:
	HonDAnimatorController()
	{
		idleClip = new HonDIdleClip();
		idleState = new HonDIdleState(idleClip, this);

		AddState(idleState);
		ChangeAnimation(idleState);
	}

	~HonDAnimatorController()
	{
		delete idleClip;
		delete idleState;
	}
};

