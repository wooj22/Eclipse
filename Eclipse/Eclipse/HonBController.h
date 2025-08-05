#pragma once
#include "HonController.h"

/* [B] Umbra �׸����� ȥ ��Ʈ�ѷ� Script */
// �������
// 1. A : �浹 �ݴ� �������� ƨ��
// 2. B : �� ������Ʈ�� 2���� �и� (ũ�� -30%, �ϰ��ӵ� +20%)
// 3. C : ����
// 4. D : �ı� 

class HonBController : public HonController
{
public:
	HonBController()
	{
		honType = HonType::B;

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

