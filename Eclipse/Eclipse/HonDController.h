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
		size = 0.5;
		descentSpeed = 128;

		// gamemanager Ãæµ¹ ºñ°Å¸® Áõ°¡
		int level = GameManager::Get().skillTree[SkillType::KnockbackDistanceUp].unlockLevel;
		if (level != 0)
		{
			SetSpeedUpRate(GameManager::Get().skillValue[SkillType::KnockbackDistanceUp][level - 1]);
		}
	}

	// script component cycle
	void Awake() override;
	void Update() override;

	// trigger event
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;

	// player attack
	void TakeDamageByPlayer() override;
};

