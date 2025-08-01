#include "GameManager.h"

void GameManager::UnInit()
{
	g_playUI = nullptr;
	//skillMap.clear();
}

void GameManager::ReSetData()
{
	honCount = 0;
	waveCount = 0;
	isWave = false;
	g_playUI = nullptr;
	skillTree.clear();

	skillTree[SkillType::KnockbackDistanceUp] = { false, 0, 3, SkillType::COUNT, 0 };
	skillTree[SkillType::DoubleJump] = { false, 0, 1, SkillType::KnockbackDistanceUp, 3 };
	skillTree[SkillType::WallJump] = { false, 0, 1, SkillType::COUNT, 0 };

	skillTree[SkillType::SkillCooldownDown] = { false, 0, 2, SkillType::COUNT, 0 };
	skillTree[SkillType::JumpAttackExtra] = { false, 0, 1, SkillType::DoubleJump, 1 };
	skillTree[SkillType::FastFall] = { false, 0, 1, SkillType::COUNT, 0 };

	skillTree[SkillType::MoveSpeedUp] = { false, 0, 3, SkillType::COUNT, 0 };
	skillTree[SkillType::AttackRangeUp] = { false, 0, 3, SkillType::COUNT, 0 };
	skillTree[SkillType::Dash] = { false, 0, 1, SkillType::MoveSpeedUp, 3 };
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

	return true; // ���� ����
}

bool GameManager::LevelUpSkill(SkillType skill)
{
	auto& info = skillTree[skill];

	if (CanUnlock(skill))
	{
		info.unlocked = true;
		info.unlockLevel = 1;
		return true;
	}
	else
	{
		if (info.unlockLevel >= info.maxLevel)
			return false;
		else 
		{
			info.unlockLevel++;
			return true;
		}
	}


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
