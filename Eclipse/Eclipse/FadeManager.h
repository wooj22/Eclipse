#pragma once
#include "../Direct2D_EngineLib/Script.h"

// Fade Manager

class ImageRenderer;
class FadeManager : public Script
{
private:
	// fade data
	float fadeSpeed = 1.0f;
	bool isFading = false;
	float currentAlpha = 1.0f;
	float targetAlpha = 1.0f;

	// ref
	ImageRenderer* ir;

public:
	// cycle
	void Awake() override;
	void Update() override;

	// func
	void FadeIn();
	void FadeOut();
};

