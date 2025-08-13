#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

/*------------ Animation Clip ------------*/
class HonCollisionEffectClip : public AnimationClip
{
public:
	HonCollisionEffectClip()
	{
		auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Hon/Texture/HonA.png");
		LoadSpriteSheetFromJson(texture, "../Resource/Woo/Hon/Data/SpriteSheet/HonA_sprites.json");
		LoadAnimationClipFromJson(texture, "../Resource/Woo/Hon/Data/AnimationClip/HonA_Idle_AniClip.json");
	}
};


/*------------ Animation State ------------*/
class HCEffect : public AnimationBaseState
{
public:
	HCEffect(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

	void Enter() override {}
	void Update(float dt) override {}
	void Exit() override {}
};



/*---------- Animation Controller ---------*/
class HCAnimatorController : public AnimatorController
{
public:
	HonCollisionEffectClip* effectClip = nullptr;
	HCEffect* effectState = nullptr;

public:
	HCAnimatorController()
	{
		effectClip = new HonCollisionEffectClip();
		effectState = new HCEffect(effectClip, this);

		AddState(effectState);
		ChangeAnimation(effectState);
	}

	~HCAnimatorController()
	{
		delete effectClip;
		delete effectState;
	}
};