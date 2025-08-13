#pragma once
#include "../Direct2D_EngineLib/Script.h"

// 월식 그림자 controller script

class Transform;
class MoonShadowController : public Script
{
private:
	Transform* tr;
	Transform* moonTr;
	float speed = 3.5f;
	bool isWaving;
	bool isBossWave;
	bool isStop;
	bool isAfterMoving;

public:
	void Awake() override;
	void Update() override;

	// 보스 웨이브 연출
	void DirectingBossWave();

	// 보스 웨이브 연출 이후 다시 이동
	void AfterMovingStart();
};

