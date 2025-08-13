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
		// ������ �����ӱ��� ����� ������ ����
		if (!clip->loop && clip->GetFrameIndexAtTime(controller->currentTime) == clip->frames.size() - 1)
		{
			controller->playing = false;
			controller->ChangeAnimation(nullptr);  // ���� �ʱ�ȭ
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
		// [ clip ���� ]
		landingClip = new LandingEffectClip();

		// [ state ���� ]
		landingEffectState = new LandingEffectState(landingClip, this);

		// [ state ��� ]
		AddState(landingEffectState);

		// �ʱ� ���� X 
		curState = nullptr;
	}

	// ����/���� �ִϸ��̼� ���
	void PlayLanding()
	{
		OutputDebugStringA("[LandingAnimatorController] Landing Animation Start\n");
		ChangeAnimation(landingEffectState);
	}

	~LandingAnimatorController() override
	{
		// [ clip ���� ]
		delete landingClip;
		delete landingEffectState;
	}
};