#pragma once
#include "../Direct2D_EngineLib/Singleton.h"

class PlayUI;
enum class SkillType {
	KnockbackDistanceUp,	// 1-1	�浹 ��Ÿ� ����	�ִ� 3
	DoubleJump,				// 1-2	���� ����			�ִ� 1
	WallJump,				// 1-3	�� ����				�ִ� 1

	SkillCooldownDown,		// 2-1	��ų ��Ÿ�� ����	�ִ� 2	
	JumpAttackExtra,		// 2-2	���� �� �߰�����	�ִ� 1
	FastFall,				// 2-3	���� ����			�ִ� 1

	MoveSpeedUp,			// 3-1	�̵��ӵ� ����		�ִ� 3
	AttackRangeUp,			// 3-2	���� �̵��Ÿ� ����	�ִ� 3
	Dash,					// 3-3	���				�ִ� 1

	COUNT					// �� ���� ������
};

// unordered_map�� Ű�� ����ϱ� ���� hash �Լ��� ����
namespace std {
	template<>
	struct hash<SkillType> {
		std::size_t operator()(const SkillType& k) const noexcept {
			return std::hash<int>()(static_cast<int>(k));
		}
	};
}

struct SkillInfo {
	bool unlocked = false;  // �ر� ����
	int unlockLevel = 0;    // �رݵ� ���� (Ȥ�� ���� ����)
	int maxLevel = 1;		// ��ų �ִ� ���� (�⺻ 1)

	// ���� ����: optional
	SkillType prerequisiteSkill = SkillType::COUNT; // �⺻�� = ����
	int requiredLevel = 0; // �� ��ų�� �� ���� �̻��� ���� �ر� ����

	SkillInfo(
		bool unlocked = false,
		int unlockLevel = 0,
		int maxLevel = 1,
		SkillType prerequisiteSkill = SkillType::COUNT,
		int requiredLevel = 0)
		: unlocked(unlocked),
		unlockLevel(unlockLevel),
		maxLevel(maxLevel),
		prerequisiteSkill(prerequisiteSkill),
		requiredLevel(requiredLevel)
	{
	}
};
class GameManager : public Singleton<GameManager>
{

public:
	PlayUI* g_playUI;
	int honCount;
	int waveCount;
	bool isWave;

	GameManager() = default;
	~GameManager() = default;

	void Init() { ReSetData(); }
	void UnInit();
	void ReSetData();
	bool CanUnlock(SkillType skill);
	bool CheckUnlock(SkillType skill) { return skillTree[skill].unlocked; }
	bool LevelUpSkill(SkillType skill);
	void AllSkillUnlock();
};