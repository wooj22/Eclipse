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

	skillTree[SkillType::KnockbackDistanceUp] = { false, 0, 3, SkillType::COUNT, 0, L"충돌 비거리 증가",L"밀려나는 영혼의 이동거리가\n        /        /    \n만큼 증가한다.",{1,2,3} ,{1,2,3} };
	skillTree[SkillType::DoubleJump] = { false, 0, 1, SkillType::KnockbackDistanceUp, 3, L"더블 점프", L"공중에서 1회 \n추가 점프가 가능해진다.",{},{2} };
	skillTree[SkillType::WallJump] = { false, 0, 1, SkillType::DoubleJump, 1, L"벽 점프",L"벽을 타고 반대방향으로\n1회 점프가 가능해 진다.",{} ,{ 3 } };

	skillTree[SkillType::SkillCooldownDown] = { false, 0, 2, SkillType::COUNT, 0, L"스킬 쿨타임 감소",L"Q와 E 스킬의 쿨타임이\n        /        초 감소한다.",{1,2} ,{1,2} };
	skillTree[SkillType::JumpAttackExtra] = { false, 0, 1, SkillType::SkillCooldownDown, 1, L"점프 후 추가공격" ,L"기본 1회 이외에\n더블점프, 벽점프 이후\n1회 추가 공격이 가능해진다." ,{},{2} };
	skillTree[SkillType::FastFall] = { false, 0, 1, SkillType::JumpAttackExtra, 1, L"빠른 낙하" ,L"아래 방향키를 눌러\n빠르게 플랫폼 위로\n내려올 수 있다." ,{},{3} };

	skillTree[SkillType::MoveSpeedUp] = { false, 0, 3, SkillType::COUNT, 0, L"이동속도 증가" ,L"이동 속도가\n        /        /    \n만큼 증가한다." , {1,3,5} ,{1,2,3} };
	skillTree[SkillType::AttackRangeUp] = { false, 0, 3, SkillType::MoveSpeedUp, 3, L"공격 이동거리 증가",L"공격 이동거리가\n        /        /    \n만큼 증가한다.", {0.5,1,1.5} ,{2,3,4} };
	skillTree[SkillType::Dash] = { false, 0, 1, SkillType::AttackRangeUp, 3, L"대시",L"Shift를 눌러\n대시를 사용할 수 있다.",{},{3} };
}

bool GameManager::CanUnlock(SkillType skill)
{
	const auto& info = skillTree[skill];
	if (info.unlocked)
		return false; // 이미 해금됨

	if (info.prerequisiteSkill != SkillType::COUNT) {
		const auto& preInfo = skillTree[info.prerequisiteSkill];
		if (preInfo.unlockLevel < info.requiredLevel)
			return false; // 조건 미달
	}

	if(info.skillCost[info.unlockLevel] > honCount)
		return false; // 혼 부족

	return true; // 조건 만족
}

bool GameManager::LevelUpSkill(SkillType skill)
{
	auto& info = skillTree[skill];

	// 스킬이 아직 잠겨있을 때
	if (!info.unlocked)
	{
		if (!CanUnlock(skill))
			return false; // 조건 미달

		honCount -= info.skillCost[info.unlockLevel];
		info.unlocked = true;
		info.unlockLevel = 1;
		return true;
	}

	// 이미 해금된 상태에서 레벨업
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
