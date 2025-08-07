#pragma once
#include "../Direct2D_EngineLib/Script.h"

 /* 맵 전경 원경 패럴랙스 스크롤링 컴포넌트 */

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

