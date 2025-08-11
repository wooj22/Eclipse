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
		duration = 0.3f;
		const std::string basePath = "../Resource/Moon/Texture/Player_Attack_Effect/";
		const int frameCount = 11;

		// 프레임별 서로 다른 시간 비중 적용
		float shortTime = 1.0f;  // 기본 가중치
		float longTime = 3.0f;   // 느리게 보이는 가중치

		// 각 프레임 가중치 배열 (총 11개)
		std::vector<float> weights;
		for (int i = 0; i < frameCount; ++i)
		{
			if (i < 4)         weights.push_back(longTime); // 앞 4프레임
			else if (i > 6)    weights.push_back(longTime); // 뒤 4프레임
			else               weights.push_back(shortTime); // 중간 3프레임
		}

		// 전체 가중치 합
		float totalWeight = 0.0f;
		for (float w : weights) totalWeight += w;

		// 가중치 비율로 프레임 시간 계산
		float accumulatedTime = 0.0f;
		for (int i = 0; i < frameCount; ++i)
		{
			char filename[256];
			sprintf_s(filename, "%sAttack (%d).png", basePath.c_str(), i);

			auto texture = ResourceManager::Get().CreateTexture2D(filename);
			auto sprite = ResourceManager::Get().CreateSprite(texture, "Attack_" + std::to_string(i));

			AnimationFrame frame;
			frame.sprite = sprite;
			frame.time = accumulatedTime;

			// 다음 프레임 시작 시간 갱신
			accumulatedTime += (weights[i] / totalWeight) * duration;

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
		//if (!clip->loop && clip->GetFrameIndexAtTime(controller->currentTime) == clip->frames.size() - 1)
		//{
		//	controller->playing = false;
		//	controller->ChangeAnimation(nullptr);  // 상태 초기화
		//}
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