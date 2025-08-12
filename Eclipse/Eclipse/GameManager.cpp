#include "GameManager.h"
#include "PlayUI.h"
#include "Boss.h"
#include "BossController.h"
#include "Quest.h"
#include "Chat.h"




void GameManager::Init()
{
	g_playUI = nullptr; 
	ReSetData();
}

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
	questCount = 0;
	questState = ChatCondition::None;
	canUseMouse = true;
	isWave = false;
	g_playUI = nullptr;
	absorbCoolTime = 0;		
	canUseAbsorb = false;		
	canUseRelease = false;
	SkillReset();
}

void GameManager::WaveStart()
{
	isWave = true;
	waveCount++;

	questCount = 0;
}

void GameManager::SkillReset()
{
	skillTree.clear();
	skillValue.clear();
	skillText.clear();

	//스킬 트리 초기화
	skillTree[SkillType::MoveSpeedUp] = { false, 0, 3, SkillType::COUNT, 0,{30,50,80} };
	skillTree[SkillType::AttackRangeUp] = { false, 0, 3, SkillType::MoveSpeedUp, 3,{30,50,80} };
	skillTree[SkillType::KnockbackDistanceUp] = { false, 0, 3, SkillType::AttackRangeUp, 3, {30,50,80} };

	skillTree[SkillType::DoubleJump] = { false, 0, 1, SkillType::COUNT, 0, {100} };
	skillTree[SkillType::WallJump] = { false, 0, 1, SkillType::DoubleJump, 1, { 100 } };
	skillTree[SkillType::JumpAttackExtra] = { false, 0, 1, SkillType::WallJump, 1,{100} };

	skillTree[SkillType::Dash] = { false, 0, 2, SkillType::COUNT, 0,{ 50,100 } };
	skillTree[SkillType::SkillCooldownDown] = { false, 0, 2, SkillType::SkillCooldownDown, 2,{50,100} };
	skillTree[SkillType::FastFall] = { false, 0, 1, SkillType::FastFall, 2,{ 100 } };


	//스킬값 초기화
	skillValue[SkillType::MoveSpeedUp] = { 1.05f,1.1f,1.15f };
	skillValue[SkillType::AttackRangeUp] = { 1.05f,1.1f,1.15f };
	skillValue[SkillType::KnockbackDistanceUp] = { 1.05f,1.1f,1.15f };

	skillValue[SkillType::DoubleJump] = { 1 };
	skillValue[SkillType::WallJump] = { 1 };
	skillValue[SkillType::JumpAttackExtra] = { 1 };

	skillValue[SkillType::Dash] = { 0.5,1 };
	skillValue[SkillType::SkillCooldownDown] = { 2,5 };
	skillValue[SkillType::FastFall] = { 1 };


	//스킬 텍스트 초기화
	skillText[SkillType::MoveSpeedUp] = { L"이동속도 증가" ,L"이동 속도가\n       /        /       %\n만큼 증가한다." };
	skillText[SkillType::AttackRangeUp] = { L"사거리 증가",L"공격 이동거리가\n       /        /       %\n만큼 증가한다." };
	skillText[SkillType::KnockbackDistanceUp] = { L"비거리 증가",L"영혼이 밀리는 거리가\n       /        /       %\n만큼 증가한다." };
	
	skillText[SkillType::DoubleJump] = { L"공중 점프", L"공중에서 한 번 더\n점프할 수 있다." };
	skillText[SkillType::WallJump] = { L"벽 점프",L"벽을 차고 한 번 더\n점프할 수 있다." };
	skillText[SkillType::JumpAttackExtra] = { L"추가 공격" ,L"공중, 벽 점프 후\n1회 추가 공격이 가능해진다." };

	skillText[SkillType::Dash] = { L"돌진 쿨타임감소",L"돌진의 재사용 대기시간이\n       /        초로 감소한다." };
	skillText[SkillType::SkillCooldownDown] = { L"흡수 쿨타임 감소",L"흡수의 재사용 대기시간이\n       /        초로 감소한다." };
	skillText[SkillType::FastFall] = { L"빠른 하강" ,L"S키를 눌러\n빠르게 하강할 수 있다." };

	

	if (g_playUI != nullptr)
	{
		g_playUI->ResetAllSkillCheat();
	}
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
		if (g_playUI != nullptr) g_playUI->ChangeHonCountText();
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
		if(g_playUI != nullptr) g_playUI->ChangeHonCountText();
	}
	return true;
}

void GameManager::AllSkillUnlock()
{
	for (auto& pair : skillTree)
	{
		SkillInfo& info = pair.second;
		info.unlocked = true;
		info.unlockLevel = info.maxLevel;
	}

	if (g_playUI !=nullptr)
	{
		g_playUI->AllSkillCheat();
	}
}

void GameManager::ChangeHonCount(int num)
{
	honCount += num;
	if (honCount < 0) honCount = 0;
	g_playUI->ChangeHonCountText();
}

void GameManager::CanAbsorb()
{
	canUseAbsorb = true;
	g_playUI->ActivateAbsorb();
	g_playUI->SkillActiveSound();
}

void GameManager::UseAbsorb()
{
	canUseAbsorb = false;
	g_playUI->DeactivateAbsorb();
}

void GameManager::CanRelease()
{
	canUseRelease = true;
	g_playUI->skill2_Image->imageRenderer->renderMode = RenderMode::Unlit;
}

void GameManager::UseRelease()
{
	canUseRelease = false;
	g_playUI->skill2_Image->imageRenderer->renderMode = RenderMode::UnlitColorTint;
}

void GameManager::FinishWave()
{
	g_playUI->timer_Text->screenTextRenderer->SetText(L"00");

	if (questState != ChatCondition::Success)
	{
		g_playUI->quest->QuestFail();
		if (waveCount == 4)
		{
			SceneManager::Get().ChangeScene(6);
			return;
		}
	}
	else
		g_playUI->quest->QuestSuccess();

	g_playUI->chat->SetCondition(questState);
}

void GameManager::ChangeQuestCount(int waveidx)
{
	if (waveidx == waveCount)
	{
		questCount++;
		g_playUI->quest->RefreshQuestCountText(questCount);
		if (questCount >= g_playUI->quest->GetQuestMaxCount())
		{
			questState = ChatCondition::Success;
			g_playUI->quest->QuestSuccess();
		}
	}
}



float GameManager::GetSkillBonus(SkillType type)
{
	auto it = skillTree.find(type);
	if (it == skillTree.end()) return 0.0f;

	int level = it->second.unlockLevel;

	auto valIt = skillValue.find(type);
	if (valIt == skillValue.end()) return 0.0f;

	const std::vector<float>& values = valIt->second;

	if (level <= 0 || values.empty()) return 0.0f;

	// 배열 범위 초과하지 않도록
	int index = clamp(level - 1, 0, static_cast<int>(values.size()) - 1);
	return values[index];
}

void GameManager::OnNPCInteraction()
{
	g_playUI->ChatSetActive(true);
	canUseMouse = false;
	if(questState == ChatCondition::Success)
		GameManager::Get().ChangeHonCount(g_playUI->quest->QuestReward());
	if (g_playUI->skillWindow_Image->IsActive())
		g_playUI->skillWindow_Image->SetActive(false);
}

void GameManager::ChangeBossHp(float hp)
{
	if(hp>0)
		g_playUI->bossHP->RefreshBossHp(hp);
	else
		g_playUI->bossHP->RefreshBossHp(0);
}
