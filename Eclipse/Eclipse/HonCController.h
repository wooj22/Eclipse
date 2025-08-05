#pragma once
#include "HonController.h"

/* [C] Darkness ����� ȥ ��Ʈ�ѷ� Script */
// �������
// 1. A : �¿�� ƨ��
// 2. B : ����
// 3. C : View���� ��� ȥ�� ������� ����
// 4. D : �ı�

class HonCController : public HonController
{
public:
	HonCController()
	{
		hp = 3;
		size = 1;
		descentSpeed = 80;
		collisionSpeed = 150;

		collisionMovingTime = 3.0f;
		pullMovingTime = 0.7f;
	}

	// script component cycle
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	// trigger event
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;
};

