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
		honType = HonType::C;

		// gamemanager Ãæµ¹ ºñ°Å¸® Áõ°¡
		int level = GameManager::Get().skillTree[SkillType::KnockbackDistanceUp].unlockLevel;
		if (level != 0)
		{
			SetSpeedUpRate(GameManager::Get().skillValue[SkillType::KnockbackDistanceUp][level - 1]);
		}
	}

	// script component cycle
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	// trigger event
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;

	// player attack
	void TakeDamageByPlayer() override;
};

