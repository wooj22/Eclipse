#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/RectTransform.h"
#include "../Direct2D_EngineLib/ScreenTextRenderer.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

class PerformanceChecker : public GameObject
{
private:
	RectTransform* rectTransform;
	ScreenTextRenderer* textRenderer;

	float timeAccumulator = 0.0f;
	int frameCount = 0;

public:
	PerformanceChecker() : GameObject("PerformanceChecker")
	{
		rectTransform = AddComponent<RectTransform>();
		textRenderer = AddComponent<ScreenTextRenderer>();

		rectTransform->SetPivot(-1, 0.5);
		rectTransform->SetPosition(-960, 200);
		rectTransform->SetSize(500,300);
		textRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
		textRenderer->layer = 50;

		textRenderer->SetEnabled(false);
	}

	void Update() override
	{
		// On/Off
		if (Input::GetKeyDown('0'))
		{
			textRenderer->SetEnabled(!textRenderer->IsEnabled());
		}

		// Trim
		if (Input::GetKeyDown('T'))
		{
			ResourceManager::Get().Trim();
		}

		// frame & memory
		timeAccumulator += Time::GetDeltaTime();
		frameCount++;

		if (timeAccumulator >= 1.0f)
		{
			int fps = frameCount;
			textRenderer->SetText(L"FPS: " + std::to_wstring(fps) +
				L"\n" + ResourceManager::Get().GetMemoryUsageWString());

			frameCount = 0;
			timeAccumulator = 0.0f;
		}
	}
};

