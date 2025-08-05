#pragma once
#include "HonController.h"

/* [C] Darkness ¾îµÒÀÇ È¥ ÄÁÆ®·Ñ·¯ Script */
// ¿¬¼â¹ÝÀÀ
// 1. A : ÁÂ¿ì·Î Æ¨±è
// 2. B : ¹«½Ã
// 3. C : View¾ÈÀÇ ¸ðµç È¥À» ²ø¾î´ç±â°í »èÁ¦
// 4. D : ÆÄ±«

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

