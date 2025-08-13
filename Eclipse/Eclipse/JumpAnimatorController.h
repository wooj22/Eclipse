#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

class JumpEffectClip : public AnimationClip
{
public:
	JumpEffectClip()
	{
		name = "Player_Jump_Effect";
		loop = false;
		duration = 0.5f;
		const std::string basePath = "../Resource/Moon/Texture/Player_Jump_Effect/";
		const int frameCount = 6;
		const float frameTime = duration / frameCount;
		for (int i = 0; i < frameCount; ++i)
		{
			char filename[256];
			sprintf_s(filename, "%sJump (%d).png", basePath.c_str(), i);

			auto texture = ResourceManager::Get().CreateTexture2D(filename);
			auto sprite = ResourceManager::Get().CreateSprite(texture, "Jump_" + std::to_string(i));

			AnimationFrame frame;
			frame.sprite = sprite;
			frame.time = i * frameTime;
			frames.push_back(frame);
		}
	}
	~JumpEffectClip() override {}
};


class JumpEffectState : public AnimationBaseState
{
public:
	JumpEffectState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

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


class JumpAnimatorController : public AnimatorController
{
	JumpEffectClip* jumpClip = nullptr;
	JumpEffectState* jumpEffectState = nullptr;

public:
	JumpAnimatorController()
	{
		// [ clip 생성 ]
		jumpClip = new JumpEffectClip();

		// [ state 생성 ]
		jumpEffectState = new JumpEffectState(jumpClip, this);

		// [ state 등록 ]
		AddState(jumpEffectState);

		// 초기 상태 X 
		curState = nullptr;
	}

	// 점프 애니메이션 재생
	void PlayJump()
	{
		// OutputDebugStringA("[JumpAnimatorController] Jump Animation Start\n");
		ChangeAnimation(jumpEffectState);
	}

	~JumpAnimatorController() override
	{
		// [ clip 삭제 ]
		delete jumpClip;
		delete jumpEffectState;
	}
};