#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

/*------------ Animation Clip ------------*/
class BossIdleClip : public AnimationClip
{
public:
	BossIdleClip()
	{
		auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Boss/Bossfire_Sheet.png");
		LoadSpriteSheetFromJson(texture, "../Resource/Woo/Boss/Bossfire_Sheet_Sprites.json");
		LoadAnimationClipFromJson(texture, "../Resource/Woo/Boss/Boss_Idle_AniClip.json");
	}
};


/*------------ Animation State ------------*/
class BossIdleState : public AnimationBaseState
{
public:
	BossIdleState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

	void Enter() override {}
	void Update(float dt) override {}
	void Exit() override {}
};



/*---------- Animation Controller ---------*/
class BossAnimatorController : public AnimatorController
{
public:
	BossIdleClip* idleClip = nullptr;
	BossIdleState* idleState = nullptr;

public:
	BossAnimatorController()
	{
		idleClip = new BossIdleClip();
		idleState = new BossIdleState(idleClip, this);

		AddState(idleState);
		ChangeAnimation(idleState);
	}

	~BossAnimatorController()
	{
		delete idleClip;
		delete idleState;
	}
};

