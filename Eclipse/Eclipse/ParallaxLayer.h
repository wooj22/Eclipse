#pragma once
#include "../Direct2D_EngineLib/Script.h"

// 배경 원경 레이어 패럴럭스 스크롤링

class Transform;
class Camera;

class ParallaxLayer : public Script
{
public:
	// 시차 계수 0.0(배경) ~ 1.0(전경)
	float parallaxFactor = 0.2;

private:
	float privousCameraX;
	float currentCameraX;

	// ref
	Transform* tr;
	Transform* maintCameraTr;

public:
	void Awake();
	void Update();
};

