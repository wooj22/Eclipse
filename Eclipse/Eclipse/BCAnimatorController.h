#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

/*------------ Animation Clip ------------*/
class BossCollisionEffectClip : public AnimationClip
{
public:
	BossCollisionEffectClip()
	{
		auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Hon/Texture/HonA.png");
		LoadSpriteSheetFromJson(texture, "../Resource/Woo/Hon/Data/SpriteSheet/HonA_sprites.json");
		LoadAnimationClipFromJson(texture, "../Resource/Woo/Hon/Data/AnimationClip/HonA_Idle_AniClip.json");
	
		loop = false;
	}
};


/*------------ Animation State ------------*/
class BCEffect : public AnimationBaseState
{
public:
	BCEffect(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

	void Enter() override {}
	void Update(float dt) override {}
	void Exit() override {}
};



/*---------- Animation Controller ---------*/
class BCAnimatorController : public AnimatorController
{
public:
	BossCollisionEffectClip* effectClip = nullptr;
	BCEffect* effectState = nullptr;

public:
	BCAnimatorController()
	{
		effectClip = new BossCollisionEffectClip();
		effectState = new BCEffect(effectClip, this);

		AddState(effectState);
		ChangeAnimation(effectState);
	}

	~BCAnimatorController()
	{
		delete effectClip;
		delete effectState;
	}
};