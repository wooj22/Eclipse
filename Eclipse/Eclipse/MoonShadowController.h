#pragma once
#include "../Direct2D_EngineLib/Script.h"

// ���� �׸��� controller script

class Transform;
class MoonShadowController : public Script
{
private:
	Transform* tr;
	float speed = 5.0f;
	bool isWaving;
	bool isBossWave;
	bool isStop;

public:
	void Awake() override;
	void Update() override;

	// ���� ���̺� ����
	void DirectingBossWave();

	// TODO :: �� ���̿� �����°� ���߿� ����
};

