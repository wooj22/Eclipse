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
		name = "HonCollisionEffect";
		loop = false;
		duration = 0.4f;

		const std::string basePath = "../Resource/Woo/Effect/monster_monster/";
		const int frameCount = 7;
		const float frameTime = duration / frameCount;

		for (int i = 0; i < frameCount; ++i)
		{
			char filename[256];
			sprintf_s(filename, "%smonster_attack_000%d.png", basePath.c_str(), i);

			auto texture = ResourceManager::Get().CreateTexture2D(filename);
			auto sprite = ResourceManager::Get().CreateSprite(texture, "HonCollisionEffect" + std::to_string(i));

			AnimationFrame frame;
			frame.sprite = sprite;
			frame.time = i * frameTime;

			frames.push_back(frame);
		}
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