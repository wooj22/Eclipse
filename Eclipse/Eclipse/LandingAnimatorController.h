#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

class LandingEffectClip : public AnimationClip
{
public:
	LandingEffectClip()
	{
		name = "Player_Landing_Effect";
		loop = false;
		duration = 0.3f;
		const std::string basePath = "../Resource/Moon/Texture/Player_Landing_Effect/";
		const int frameCount = 8;
		const float frameTime = duration / frameCount;
		for (int i = 0; i < frameCount; ++i)
		{
			char filename[256];
			sprintf_s(filename, "%sLanding (%d).png", basePath.c_str(), i);

			auto texture = ResourceManager::Get().CreateTexture2D(filename);
			auto sprite = ResourceManager::Get().CreateSprite(texture, "Landing_" + std::to_string(i));

			AnimationFrame frame;
			frame.sprite = sprite;
			frame.time = i * frameTime;
			frames.push_back(frame);
		}
	}
	~LandingEffectClip() override {}
};


class LandingEffectState : public AnimationBaseState
{
public:
	LandingEffectState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

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


class LandingAnimatorController : public AnimatorController
{
	LandingEffectClip* landingClip = nullptr;
	LandingEffectState* landingEffectState = nullptr;

public:
	LandingAnimatorController()
	{
		// [ clip 생성 ]
		landingClip = new LandingEffectClip();

		// [ state 생성 ]
		landingEffectState = new LandingEffectState(landingClip, this);

		// [ state 등록 ]
		AddState(landingEffectState);

		// 초기 상태 X 
		curState = nullptr;
	}

	// 점프/착지 애니메이션 재생
	void PlayLanding()
	{
		OutputDebugStringA("[LandingAnimatorController] Landing Animation Start\n");
		ChangeAnimation(landingEffectState);
	}

	~LandingAnimatorController() override
	{
		// [ clip 삭제 ]
		delete landingClip;
		delete landingEffectState;
	}
};