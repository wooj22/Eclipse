#pragma once
#include "HonController.h"

/* [A] Ignis ���� ȥ ��Ʈ�ѷ� Script */
// �������
// 1. A : �� ������Ʈ�� ������ (ũ�� +10%, �ϰ��ӵ� -20%)
// 2. B : �浹 �ݴ� �������� ƨ��
// 3. C : �¿�� ƨ��
// 4. D : �ı�

class HonAController : public HonController
{
public:
	HonAController()
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

