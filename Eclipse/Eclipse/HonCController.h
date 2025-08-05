#pragma once
#include "HonController.h"

/* [C] Darkness ����� ȥ ��Ʈ�ѷ� Script */
// �������
// 1. A : �¿�� ƨ��
// 2. B : ����
// 3. C : View���� ��� ȥ�� ������� ����
// 4. D : �ı�

class HonCController : public HonController
{
public:
	HonCController()
	{
		honType = HonType::C;

		hp = 3;
		size = 1;
		descentSpeed = 80;

		// gamemanager �浹 ��Ÿ� ����
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

