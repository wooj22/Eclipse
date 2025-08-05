#pragma once
#include "HonController.h"

/* [D] Luna �޺��� ȥ ��Ʈ�ѷ� Script */
// �������
// 1. A : �ı�
// 2. B : �ı�
// 3. C : �ı�
// 4. D : �ı�

class HonDController : public HonController
{
public:
	HonDController()
	{
		hp = 1;
		size = 1;
		descentSpeed = 150;
		collisionSpeed = 150;

		collisionMovingTime = 3.0f;
		pullMovingTime = 0.7f;
	}

	// script component cycle
	void Awake() override;
	void Update() override;

	// trigger event
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;
};

