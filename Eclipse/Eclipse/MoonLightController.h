#pragma once
#include "../Direct2D_EngineLib/Script.h"

// ¿ù½Ä ºû controller script

class Transform;
class MoonLightController : public Script
{
private:
	// data
	float scaleSpeed = 0.2f;
	bool increasing;

	// ref
	Transform* tr;

public:
	void Awake() override;
	void Update() override;
};

