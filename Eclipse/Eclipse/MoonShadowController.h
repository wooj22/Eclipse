#pragma once
#include "../Direct2D_EngineLib/Script.h"

// 월식 그림자 controller script

class Transform;
class MoonShadowController : public Script
{
private:
	Transform* tr;
	Transform* moonTr;
	float speed = 8.0f;
	bool isWaving;
	bool isBossWave;
	bool isStop;

public:
	void Awake() override;
	void Update() override;

	// 보스 웨이브 연출
	void DirectingBossWave();
};

