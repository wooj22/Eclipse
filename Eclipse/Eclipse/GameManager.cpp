#include "GameManager.h"

void GameManager::UnInit()
{
	g_playUI = nullptr;
	skillTree.clear();
}

void GameManager::ReSetData()
{
	honCount = 0;
	waveCount = 0;
	isWave = false;
	g_playUI = nullptr;
	skillTree.clear();

	skillTree[SkillType::KnockbackDistanceUp] = { false, 0, 3, SkillType::COUNT, 0, L"�浹 ��Ÿ� ����",L"�з����� ��ȥ�� �̵��Ÿ���\n        /        /    \n��ŭ �����Ѵ�.",{1,2,3} ,{1,2,3} };
	skillTree[SkillType::DoubleJump] = { false, 0, 1, SkillType::KnockbackDistanceUp, 3, L"���� ����", L"���߿��� 1ȸ \n�߰� ������ ����������.",{},{2} };
	skillTree[SkillType::WallJump] = { false, 0, 1, SkillType::DoubleJump, 1, L"�� ����",L"���� Ÿ�� �ݴ��������\n1ȸ ������ ������ ����.",{} ,{ 3 } };

	skillTree[SkillType::SkillCooldownDown] = { false, 0, 2, SkillType::COUNT, 0, L"��ų ��Ÿ�� ����",L"Q�� E ��ų�� ��Ÿ����\n        /        �� �����Ѵ�.",{1,2} ,{1,2} };
	skillTree[SkillType::JumpAttackExtra] = { false, 0, 1, SkillType::SkillCooldownDown, 1, L"���� �� �߰�����" ,L"�⺻ 1ȸ �̿ܿ�\n��������, ������ ����\n1ȸ �߰� ������ ����������." ,{},{2} };
	skillTree[SkillType::FastFall] = { false, 0, 1, SkillType::JumpAttackExtra, 1, L"���� ����" ,L"�Ʒ� ����Ű�� ����\n������ �÷��� ����\n������ �� �ִ�." ,{},{3} };

	skillTree[SkillType::MoveSpeedUp] = { false, 0, 3, SkillType::COUNT, 0, L"�̵��ӵ� ����" ,L"�̵� �ӵ���\n        /        /    \n��ŭ �����Ѵ�." , {1,3,5} ,{1,2,3} };
	skillTree[SkillType::AttackRangeUp] = { false, 0, 3, SkillType::MoveSpeedUp, 3, L"���� �̵��Ÿ� ����",L"���� �̵��Ÿ���\n        /        /    \n��ŭ �����Ѵ�.", {0.5,1,1.5} ,{2,3,4} };
	skillTree[SkillType::Dash] = { false, 0, 1, SkillType::AttackRangeUp, 3, L"���",L"Shift�� ����\n��ø� ����� �� �ִ�.",{},{3} };
}

bool GameManager::CanUnlock(SkillType skill)
{
	const auto& info = skillTree[skill];
	if (info.unlocked)
		return false; // �̹� �رݵ�

	if (info.prerequisiteSkill != SkillType::COUNT) {
		const auto& preInfo = skillTree[info.prerequisiteSkill];
		if (preInfo.unlockLevel < info.requiredLevel)
			return false; // ���� �̴�
	}

	if(info.skillCost[info.unlockLevel] > honCount)
		return false; // ȥ ����

	return true; // ���� ����
}

bool GameManager::LevelUpSkill(SkillType skill)
{
	auto& info = skillTree[skill];

	// ��ų�� ���� ������� ��
	if (!info.unlocked)
	{
		if (!CanUnlock(skill))
			return false; // ���� �̴�

		honCount -= info.skillCost[info.unlockLevel];
		info.unlocked = true;
		info.unlockLevel = 1;
		return true;
	}

	// �̹� �رݵ� ���¿��� ������
	if (info.unlockLevel >= info.maxLevel)
		return false;

	honCount -= info.skillCost[info.unlockLevel];
	info.unlockLevel++;
	return true;
}

void GameManager::AllSkillUnlock()
{
	if (g_playUI == nullptr)
		return;
	for (auto& pair : skillTree)
	{
		SkillInfo& info = pair.second;
		info.unlocked = true;
		info.unlockLevel = info.maxLevel;
	}
}
