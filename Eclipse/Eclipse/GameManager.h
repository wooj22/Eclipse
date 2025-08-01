#pragma once
#include "../Direct2D_EngineLib/Singleton.h"
#include <unordered_map>
#include <string>

class PlayUI;


enum class SkillType {
	KnockbackDistanceUp,
	DoubleJump,
	WallJump,
	SkillCooldownDown,
	JumpAttackExtra,
	FastFall,
	MoveSpeedUp,
	AttackRangeUp,
	Dash,
	COUNT // �� ���� ������
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
	std::unordered_map<SkillType, SkillInfo> skillTree;

	GameManager() = default;
	~GameManager() = default;

	void Init() { ReSetData(); }
	void UnInit();
	void ReSetData();
	bool CanUnlock(SkillType skill);
	bool LevelUpSkill(SkillType skill);
};