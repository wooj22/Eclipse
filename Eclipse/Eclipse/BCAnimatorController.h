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
		name = "BossCollisionEffect";
		loop = false;
		duration = 0.3f;

		const std::string basePath = "../Resource/Woo/Effect/boss_monster/";
		const int frameCount = 4;
		const float frameTime = duration / frameCount;

		for (int i = 0; i < frameCount; ++i)
		{
			char filename[256];
			sprintf_s(filename, "%sboss_attack_000%d.png", basePath.c_str(), i);

			auto texture = ResourceManager::Get().CreateTexture2D(filename);
			auto sprite = ResourceManager::Get().CreateSprite(texture, "BossCollisionEffect" + std::to_string(i));

			AnimationFrame frame;
			frame.sprite = sprite;
			frame.time = i * frameTime;

			frames.push_back(frame);
		}
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