#pragma once
#include "HonController.h"

/* [B] Umbra 그림자의 혼 컨트롤러 Script */
// 연쇄반응
// 1. A : 충돌 반대 방향으로 튕김
// 2. B : 각 오브젝트가 2개로 분리 (크기 -30%, 하강속도 +20%)
// 3. C : 무시
// 4. D : 파괴 

class HonBController : public HonController
{
private:
	// hon C-B 겹침 x
	Vector2 honBC_dir;

public:
	HonBController()
	{
		honType = HonType::B;

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
	void OnTriggerStay(ICollider* other, const ContactInfo& contact) override;

	// player attack
	void TakeDamageByPlayer() override;
};

