#pragma once
#include "HonController.h"

/* [D] Luna ´ÞºûÀÇ È¥ ÄÁÆ®·Ñ·¯ Script */
// ¿¬¼â¹ÝÀÀ
// 1. A : ÆÄ±«
// 2. B : ÆÄ±«
// 3. C : ÆÄ±«
// 4. D : ÆÄ±«

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

