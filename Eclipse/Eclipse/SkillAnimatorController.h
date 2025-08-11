#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"


class SkillEffectClip : public AnimationClip
{
public:
	SkillEffectClip()
	{
		name = "Player_Skill_Effect";
		loop = false;
		duration = 0.25f;
		const std::string basePath = "../Resource/Moon/Texture/Player_Skill_Effect/";
		const int frameCount = 8;
		const float frameTime = duration / frameCount;
		for (int i = 0; i < frameCount; ++i)
		{
			char filename[256];
			sprintf_s(filename, "%sSkill (%d).png", basePath.c_str(), i);

			auto texture = ResourceManager::Get().CreateTexture2D(filename);
			auto sprite = ResourceManager::Get().CreateSprite(texture, "Skill_" + std::to_string(i));

			AnimationFrame frame;
			frame.sprite = sprite;
			frame.time = i * frameTime;
			frames.push_back(frame);
		}
	}
	~SkillEffectClip() override {}
};


class SkillEffectState : public AnimationBaseState
{
public:
	SkillEffectState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

	void Enter() override {}

	void Update(float dt) override
	{
		// 마지막 프레임까지 재생이 끝나면 종료
		if (!clip->loop && clip->GetFrameIndexAtTime(controller->currentTime) == clip->frames.size() - 1)
		{
			controller->playing = false;
			controller->ChangeAnimation(nullptr);  // 상태 초기화
		}
	}
	void Exit() override {}
};


class SkillAnimatorController : public AnimatorController
{
	SkillEffectClip* skillClip = nullptr;
	SkillEffectState* skillEffectState = nullptr;

public:
	SkillAnimatorController()
	{
		// [ clip 생성 ]
		skillClip = new SkillEffectClip();

		// [ state 생성 ]
		skillEffectState = new SkillEffectState(skillClip, this);

		// [ state 등록 ]
		AddState(skillEffectState);

		// 초기 상태 X 
		curState = nullptr;
	}

	// 스킬 애니메이션 재생
	void PlaySkill()
	{
		ChangeAnimation(skillEffectState);
	}

	~SkillAnimatorController() override
	{
		// [ clip 삭제 ]
		delete skillClip;
		delete skillEffectState;
	}
};