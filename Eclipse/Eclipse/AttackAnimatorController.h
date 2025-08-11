#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"


class AttackEffectClip : public AnimationClip
{
public:
	AttackEffectClip()
	{
		name = "Player_Attack_Effect";
		loop = false;
		duration = 0.8f;
		const std::string basePath = "../Resource/Moon/Texture/Player_Attack_Effect/";
		const int frameCount = 11;
		const float frameTime = duration / frameCount;
		for (int i = 0; i < frameCount; ++i)
		{
			char filename[256];
			sprintf_s(filename, "%sAttack (%d).png", basePath.c_str(), i);

			auto texture = ResourceManager::Get().CreateTexture2D(filename);
			auto sprite = ResourceManager::Get().CreateSprite(texture, "Attack_" + std::to_string(i));

			AnimationFrame frame;
			frame.sprite = sprite;
			frame.time = i * frameTime;
			frames.push_back(frame);
		}
	}
	~AttackEffectClip() override {}
};



class AttackEffectState : public AnimationBaseState
{
public:
	AttackEffectState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

	void Enter() override {}

	void Update(float dt) override
	{
		// if (controller->GetBool("Player_Attack_Effect") == true)  controller->PlayAnimation("Player_Attack_Effect");

		// 마지막 프레임까지 재생이 끝나면 종료
		if (!clip->loop && clip->GetFrameIndexAtTime(controller->currentTime) == clip->frames.size() - 1)
		{
			controller->playing = false;
			controller->ChangeAnimation(nullptr);  // 상태 초기화
		}
	}
	void Exit() override {}
};


class AttackAnimatorController : public AnimatorController
{
	AttackEffectClip* attackClip = nullptr;
	AttackEffectState* attackEffectState = nullptr;

public:
	AttackAnimatorController()
	{
		// [ clip 생성 ]
		attackClip = new AttackEffectClip();

		// [ state 생성 ]
		attackEffectState = new AttackEffectState(attackClip, this);
		
		// [ state 등록 ]
		AddState(attackEffectState);

		// 초기 상태 X 
		curState = nullptr;
	}

	// 공격 애니메이션 재생
	void PlayAttack()
	{
		ChangeAnimation(attackEffectState);
	}

	~AttackAnimatorController() override
	{
		// [ clip 삭제 ]
		delete attackClip;
		delete attackEffectState;
	}
};