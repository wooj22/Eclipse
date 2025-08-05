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

