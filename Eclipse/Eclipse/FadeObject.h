#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/RectTransform.h"
#include "../Direct2D_EngineLib/ImageRenderer.h"
#include "FadeManager.h"

// Fade Util GameObject

class FadeObject : public GameObject
{
public:
	RectTransform* rectTransform = nullptr;
	ImageRenderer* imageRenderer = nullptr;
	FadeManager* fadeManager = nullptr;

	FadeObject() : GameObject("FadeManager")
	{
		rectTransform = AddComponent<RectTransform>();
		imageRenderer = AddComponent<ImageRenderer>();
		fadeManager = AddComponent<FadeManager>();

		rectTransform->SetSize(1920, 1080);
		imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::Black));
		imageRenderer->layer = 30;
	}
};

