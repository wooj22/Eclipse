#pragma once
#include "../Direct2D_EngineLib/Script.h"

// ¿ù½Ä ºû controller script

class Transform;
class MoonLightController : public Script
{
private:
	// data
	float scaleSpeed = 0.05f;

	// ref
	Transform* tr;

public:
	void Awake() override;
	void Update() override;
};

