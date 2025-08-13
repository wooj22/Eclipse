#pragma once
#include "../Direct2D_EngineLib/Script.h"

// ���� �׸��� controller script

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

	// ���� ���̺� ����
	void DirectingBossWave();

	// ���� ���̺� ���� ���� �ٽ� �̵�
	void AfterMovingStart();
};

