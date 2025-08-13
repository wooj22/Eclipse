#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"

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

private:
	SpriteRenderer* spriteRenderer;

	void Enter() override
    {
		spriteRenderer = GameObject::Find("PlayerLandingEffect")->GetComponent<SpriteRenderer>();

        if (spriteRenderer)  spriteRenderer->SetAlpha(1.0f); // 알파 초기화
    }

    void Update(float dt) override
    {
		if (!spriteRenderer) return;

		int frameIndex = clip->GetFrameIndexAtTime(controller->currentTime);
		int totalFrames = static_cast<int>(clip->frames.size());

		float alpha = 1.0f;

		if (frameIndex >= 2) // 3번째 프레임부터 감소 
		{
			int fadeFrames = totalFrames - 3; // 3번째부터 마지막 전 프레임까지
			float t = static_cast<float>(frameIndex - 2) / fadeFrames; // 0~1
			alpha = 1.0f - t * 0.9f; // 마지막 프레임 0.1
		}

		spriteRenderer->SetAlpha(alpha);

		if (!clip->loop && frameIndex == totalFrames - 1)
		{
			controller->playing = false;
			controller->ChangeAnimation(nullptr);
			spriteRenderer->SetAlpha(1.0f);
		}
    }

    void Exit() override
    {
        if (spriteRenderer)  spriteRenderer->SetAlpha(1.0f); // 알파 복귀
    }
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