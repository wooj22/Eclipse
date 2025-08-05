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
		honType = HonType::D;

		hp = 1;
		size = 1;
		descentSpeed = 150;
		collisionSpeed = 1000;

		collisionMovingTime = 1.5f;
	}

	// script component cycle
	void Awake() override;
	void Update() override;

	// trigger event
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;
};

