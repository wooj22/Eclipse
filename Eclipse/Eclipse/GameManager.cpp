#include "GameManager.h"
#include "PlayUI.h"

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
	honKillCount = 0;
	cainCount = 0;
	lunaKillCount = 0;
	bossKillCount = 0;
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

	honKillCount = 0;
	cainCount = 0;
	lunaKillCount = 0;
	bossKillCount = 0;
}

void GameManager::SkillReset()
{
	skillTree.clear();
	skillValue.clear();
	skillText.clear();

	//��ų Ʈ�� �ʱ�ȭ
	skillTree[SkillType::KnockbackDistanceUp] = { false, 0, 3, SkillType::COUNT, 0, {5,8,10} };
	skillTree[SkillType::DoubleJump] = { false, 0, 1, SkillType::KnockbackDistanceUp, 3, {10} };
	skillTree[SkillType::WallJump] = { false, 0, 1, SkillType::DoubleJump, 1, { 15 } };

	skillTree[SkillType::SkillCooldownDown] = { false, 0, 2, SkillType::COUNT, 0,{5,10} };
	skillTree[SkillType::JumpAttackExtra] = { false, 0, 1, SkillType::SkillCooldownDown, 1,{10} };
	skillTree[SkillType::FastFall] = { false, 0, 1, SkillType::JumpAttackExtra, 1,{15} };

	skillTree[SkillType::MoveSpeedUp] = { false, 0, 3, SkillType::COUNT, 0,{5,8,10} };
	skillTree[SkillType::AttackRangeUp] = { false, 0, 3, SkillType::MoveSpeedUp, 3,{5,8,10} };
	skillTree[SkillType::Dash] = { false, 0, 1, SkillType::AttackRangeUp, 3,{15} };

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

void GameManager::FinishWaveTimeText()
{
	g_playUI->timer_Text->screenTextRenderer->SetText(L"00");
}