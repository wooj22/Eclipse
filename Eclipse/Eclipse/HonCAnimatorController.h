#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

/*------------ Animation Clip ------------*/
class HonCIdleClip : public AnimationClip
{
public:
	HonCIdleClip()
	{
		auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Hon/Texture/HonC.png");
		LoadSpriteSheetFromJson(texture, "../Resource/Woo/Hon/Data/SpriteSheet/HonC_Sprites.json");
		LoadAnimationClipFromJson(texture, "../Resource/Woo/Hon/Data/AnimationClip/HonC_Idle_AniClip.json");
	}
};





/*------------ Animation State ------------*/
class HonCIdleState : public AnimationBaseState
{
public:
	HonCIdleState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

	void Enter() override {}
	void Update(float dt) override {}
	void Exit() override {}
};






/*---------- Animation Controller ---------*/
class HonCAnimatorController : public AnimatorController
{
public:
	HonCIdleClip* idleClip = nullptr;
	HonCIdleState* idleState = nullptr;

public:
	HonCAnimatorController()
	{
		idleClip = new HonCIdleClip();
		idleState = new HonCIdleState(idleClip, this);

		AddState(idleState);
		ChangeAnimation(idleState);
	}

	~HonCAnimatorController()
	{
		delete idleClip;
		delete idleState;
	}
};

