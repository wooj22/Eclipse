#pragma once
#include "HonController.h"

/* [C] Darkness 어둠의 혼 컨트롤러 Script */
// 연쇄반응
// 1. A : 좌우로 튕김
// 2. B : 무시
// 3. C : View안의 모든 혼을 끌어당기고 삭제
// 4. D : 파괴

class HonCController : public HonController
{
public:
	HonCController()
	{
		honType = HonType::C;

		// gamemanager 충돌 비거리 증가
		int level = GameManager::Get().skillTree[SkillType::KnockbackDistanceUp].unlockLevel;
		if (level != 0)
		{
			//SetSpeedUpRate(GameManager::Get().skillValue[SkillType::KnockbackDistanceUp][level - 1]);
		}
	}

	// script component cycle
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	// trigger event
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;
};

