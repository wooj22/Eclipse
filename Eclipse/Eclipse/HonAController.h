#pragma once
#include "HonController.h"

/* [A] Ignis 불의 혼 컨트롤러 Script */
// 연쇄반응
// 1. A : 두 오브젝트가 합쳐짐 (크기 +10%, 하강속도 -20%)
// 2. B : 충돌 반대 방향으로 튕김
// 3. C : 좌우로 튕김
// 4. D : 파괴

class HonAController : public HonController
{
public:
	float is2A;

public:
	HonAController()
	{
		honType = HonType::A;

		hp = 3;
		size = 1;
		descentSpeed = 80;

		// gamemanager 충돌 비거리 증가
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
};

