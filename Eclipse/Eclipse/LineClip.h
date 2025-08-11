#pragma once
#include "../Direct2D_EngineLib/AnimationClip.h"
#include "../Direct2D_EngineLib/AnimationBaseState.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

class LineClip : public AnimationClip
{
public:
	LineClip()
	{
		name = "UI_Line";
		loop = false;
		duration = 0.5f;

		const std::string basePath = "../Resource/mo/UI_Line/";
		const int frameCount = 30;
		const float frameTime = duration / frameCount;

		for (int i = 0; i < frameCount; ++i)
		{
			char filename[256];
			sprintf_s(filename, "%sUI_Line (%d).png", basePath.c_str(), i);

			auto texture = ResourceManager::Get().CreateTexture2D(filename);
			auto sprite = ResourceManager::Get().CreateSprite(texture, "UI_Line" + std::to_string(i));

			AnimationFrame frame;
			frame.sprite = sprite;
			frame.time = i * frameTime;

			frames.push_back(frame);
		}
	}
	~LineClip() override {}
};

class LineState : public AnimationBaseState
{
public:
	LineState(AnimationClip* c, AnimatorController* ac) : AnimationBaseState(c, ac) {}

	void Enter() override {}
	void Update(float dt) override {}
	void Exit() override {}
};

class LineController : public AnimatorController
{
public:
	LineClip* lineClip = nullptr;
	LineState* lineState = nullptr;
public:
	LineController()
	{
		lineClip = new LineClip();
		lineState = new LineState(lineClip, this);

		AddState(lineState);
		ChangeAnimation(lineState);
	}
	~LineController()
	{
		delete lineClip;
		delete lineState;
	}
};