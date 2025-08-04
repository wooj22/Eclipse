#include "GameManager.h"

void GameManager::UnInit()
{
	g_playUI = nullptr;
	skillTree.clear();
	skillValue.clear();
	skillText.clear();
}

void GameManager::ReSetData()	
{
	honCount = 0;
	waveCount = 0;
	isWave = false;
	g_playUI = nullptr;
	skillTree.clear();
	skillValue.clear();
	skillText.clear();

	//스킬 트리 초기화
	skillTree[SkillType::KnockbackDistanceUp] = { false, 0, 3, SkillType::COUNT, 0, {5,8,10} };
	skillTree[SkillType::DoubleJump] = { false, 0, 1, SkillType::KnockbackDistanceUp, 3, {10} };
	skillTree[SkillType::WallJump] = { false, 0, 1, SkillType::DoubleJump, 1, { 15 } };

	skillTree[SkillType::SkillCooldownDown] = { false, 0, 2, SkillType::COUNT, 0,{5,10} };
	skillTree[SkillType::JumpAttackExtra] = { false, 0, 1, SkillType::SkillCooldownDown, 1,{10} };
	skillTree[SkillType::FastFall] = { false, 0, 1, SkillType::JumpAttackExtra, 1,{15} };

	skillTree[SkillType::MoveSpeedUp] = { false, 0, 3, SkillType::COUNT, 0,{5,8,10} };
	skillTree[SkillType::AttackRangeUp] = { false, 0, 3, SkillType::MoveSpeedUp, 3,{5,8,10} };
	skillTree[SkillType::Dash] = { false, 0, 1, SkillType::AttackRangeUp, 3,{15} };
	
	//스킬값 초기화
	skillValue[SkillType::KnockbackDistanceUp] = {1.05f,1.1f,1.15f};
	skillValue[SkillType::DoubleJump] = { 1 };
	skillValue[SkillType::WallJump] = { 1 };

	skillValue[SkillType::SkillCooldownDown] = { 2,5 };
	skillValue[SkillType::JumpAttackExtra] = { 1 };
	skillValue[SkillType::FastFall] = { 1 };

	skillValue[SkillType::MoveSpeedUp] = { 1.05f,1.1f,1.15f };
	skillValue[SkillType::AttackRangeUp] = { 1.05f,1.1f,1.15f };
	skillValue[SkillType::Dash] = { 1 };

	//스킬 텍스트 초기화
	skillText[SkillType::KnockbackDistanceUp] = {L"충돌 비거리 증가",L"밀려나는 영혼의 이동거리가\n        /        /       %\n만큼 증가한다."};
	skillText[SkillType::DoubleJump] = { L"더블 점프", L"공중에서 1회 \n추가 점프가 가능해진다."};
	skillText[SkillType::WallJump] = { L"벽 점프",L"벽을 타고 반대방향으로\n1회 점프가 가능해 진다."};

	skillText[SkillType::SkillCooldownDown] = { L"스킬 쿨타임 감소",L"Q와 E 스킬의 쿨타임이\n        /        초 감소한다." };
	skillText[SkillType::JumpAttackExtra] = { L"점프 후 추가공격" ,L"기본 1회 이외에\n더블점프, 벽점프 이후\n1회 추가 공격이 가능해진다."  };
	skillText[SkillType::FastFall] = { L"빠른 낙하" ,L"아래 방향키를 눌러\n빠르게 플랫폼 위로\n내려올 수 있다."  };

	skillText[SkillType::MoveSpeedUp] = { L"이동속도 증가" ,L"이동 속도가\n        /        /       %\n만큼 증가한다."  };
	skillText[SkillType::AttackRangeUp] = { L"공격 이동거리 증가",L"공격 이동거리가\n        /        /       %\n만큼 증가한다." };
	skillText[SkillType::Dash] = {  L"대시",L"Shift를 눌러\n대시를 사용할 수 있다." };
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

bool GameManager::LevelUpSkill(SkillType skill, bool check )
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

	// 혼 부족
	if (info.skillCost[info.unlockLevel] > honCount)
		return false; 

	if (!check)
	{
		honCount -= info.skillCost[info.unlockLevel];
		info.unlockLevel++;
	}
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
