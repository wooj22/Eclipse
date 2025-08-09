#include "GameManager.h"
#include "PlayUI.h"
#include "Boss.h"
#include "BossController.h"
#include "Quest.h"
#include "Chat.h"


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

	//��ų Ʈ�� �ʱ�ȭ
	skillTree[SkillType::KnockbackDistanceUp] = { false, 0, 3, SkillType::COUNT, 0, {15,20,30} };
	skillTree[SkillType::DoubleJump] = { false, 0, 1, SkillType::KnockbackDistanceUp, 3, {25} };
	skillTree[SkillType::WallJump] = { false, 0, 1, SkillType::DoubleJump, 1, { 30 } };

	skillTree[SkillType::SkillCooldownDown] = { false, 0, 2, SkillType::COUNT, 0,{15,30} };
	skillTree[SkillType::JumpAttackExtra] = { false, 0, 1, SkillType::SkillCooldownDown, 1,{25} };
	skillTree[SkillType::FastFall] = { false, 0, 1, SkillType::JumpAttackExtra, 1,{ 30 } };

	skillTree[SkillType::MoveSpeedUp] = { false, 0, 3, SkillType::COUNT, 0,{15,20,30} };
	skillTree[SkillType::AttackRangeUp] = { false, 0, 3, SkillType::MoveSpeedUp, 3,{15,20,30} };
	skillTree[SkillType::Dash] = { false, 0, 1, SkillType::AttackRangeUp, 3,{ 30 } };

	//��ų�� �ʱ�ȭ
	skillValue[SkillType::KnockbackDistanceUp] = { 1.05f,1.1f,1.15f };
	skillValue[SkillType::DoubleJump] = { 1 };
	skillValue[SkillType::WallJump] = { 1 };

	skillValue[SkillType::SkillCooldownDown] = { 2,5 };
	skillValue[SkillType::JumpAttackExtra] = { 1 };
	skillValue[SkillType::FastFall] = { 1 };

	skillValue[SkillType::MoveSpeedUp] = { 1.05f,1.1f,1.15f };
	skillValue[SkillType::AttackRangeUp] = { 1.05f,1.1f,1.15f };
	skillValue[SkillType::Dash] = { 1 };

	//��ų �ؽ�Ʈ �ʱ�ȭ
	skillText[SkillType::KnockbackDistanceUp] = { L"�浹 ��Ÿ� ����",L"�з����� ��ȥ�� �̵��Ÿ���\n        /        /       %\n��ŭ �����Ѵ�." };
	skillText[SkillType::DoubleJump] = { L"���� ����", L"���߿��� 1ȸ \n�߰� ������ ����������." };
	skillText[SkillType::WallJump] = { L"�� ����",L"���� Ÿ�� �ݴ��������\n1ȸ ������ ������ ����." };

	skillText[SkillType::SkillCooldownDown] = { L"��ų ��Ÿ�� ����",L"Q�� E ��ų�� ��Ÿ����\n        /        �� �����Ѵ�." };
	skillText[SkillType::JumpAttackExtra] = { L"���� �� �߰�����" ,L"�⺻ 1ȸ �̿ܿ�\n��������, ������ ����\n1ȸ �߰� ������ ����������." };
	skillText[SkillType::FastFall] = { L"���� ����" ,L"�Ʒ� ����Ű�� ����\n������ �÷��� ����\n������ �� �ִ�." };

	skillText[SkillType::MoveSpeedUp] = { L"�̵��ӵ� ����" ,L"�̵� �ӵ���\n        /        /       %\n��ŭ �����Ѵ�." };
	skillText[SkillType::AttackRangeUp] = { L"���� �̵��Ÿ� ����",L"���� �̵��Ÿ���\n        /        /       %\n��ŭ �����Ѵ�." };
	skillText[SkillType::Dash] = { L"���",L"Shift�� ����\n��ø� ����� �� �ִ�." };

	if (g_playUI != nullptr)
	{
		g_playUI->ResetAllSkillCheat();
	}
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

bool GameManager::LevelUpSkill(SkillType skill, bool check )
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
		if (g_playUI != nullptr) g_playUI->ChangeHonCountText();
		return true;
	}

	// �̹� �رݵ� ���¿��� ������
	if (info.unlockLevel >= info.maxLevel)
		return false;

	// ȥ ����
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

	// �迭 ���� �ʰ����� �ʵ���
	int index = clamp(level - 1, 0, static_cast<int>(values.size()) - 1);
	return values[index];
}

void GameManager::OnNPCInteraction()
{
	g_playUI->ChatSetActive(true);
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
