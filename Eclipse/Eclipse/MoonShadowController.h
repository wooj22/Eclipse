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
	bool isWaving = false;
	bool isBossWave = false;
	bool isStop = false;
	bool isAfterMoving = false;
	bool isDirecting = false;

public:
	void Awake() override;
	void Update() override;

	// 월식 연출
	void DirectingBossWave();

	// 월식 연출 이후 다시 이동
	void AfterMovingStart();

	// 월식 연출 진행중 플래그
	bool IsEclipseDirecting() { return isDirecting; }

	// 월식 연출 restart
	void ReStart();
};

