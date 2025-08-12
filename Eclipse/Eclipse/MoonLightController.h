#pragma once
#include "../Direct2D_EngineLib/Script.h"

// 월식 빛 controller script

class Transform;
class MoonLightController : public Script
{
private:
	// data
	bool increasing = true;			  // 커지는 중인지
	float scaleSpeed = 0.05f;         // 초당 크기 변화량

	// ref
	Transform* tr;

public:
	void Awake() override;
	void Update() override;
};

