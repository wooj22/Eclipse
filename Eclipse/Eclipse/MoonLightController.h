#pragma once
#include "../Direct2D_EngineLib/Script.h"

// ���� �� controller script

class Transform;
class MoonLightController : public Script
{
private:
	// data
	bool increasing = true;			  // Ŀ���� ������
	float scaleSpeed = 0.05f;         // �ʴ� ũ�� ��ȭ��

	// ref
	Transform* tr;

public:
	void Awake() override;
	void Update() override;
};

