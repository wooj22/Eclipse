#pragma once
#include "../Direct2D_EngineLib/Singleton.h"
#include <unordered_map>
#include <string>

class PlayUI;


enum class SkillType {
	KnockbackDistanceUp,	// 1-1	충돌 비거리 증가	최대 3
	DoubleJump,				// 1-2	더블 점프			최대 1
	WallJump,				// 1-3	벽 점프				최대 1

	SkillCooldownDown,		// 2-1	스킬 쿨타임 감소	최대 2	
	JumpAttackExtra,		// 2-2	점프 후 추가공격	최대 1
	FastFall,				// 2-3	빠른 낙하			최대 1

	MoveSpeedUp,			// 3-1	이동속도 증가		최대 3
	AttackRangeUp,			// 3-2	공격 이동거리 증가	최대 3
	Dash,					// 3-3	대시				최대 1

	COUNT					// 총 개수 관리용
};

// unordered_map의 키로 사용하기 위해 hash 함수를 정의
namespace std {
	template<>
	struct hash<SkillType> {
		std::size_t operator()(const SkillType& k) const noexcept {
			return std::hash<int>()(static_cast<int>(k));
		}
	};
}

struct SkillInfo {
	bool unlocked = false;  // 해금 여부
	int unlockLevel = 0;    // 해금된 레벨 (혹은 조건 레벨)
	int maxLevel = 1;		// 스킬 최대 레벨 (기본 1)

	// 선행 조건: optional
	SkillType prerequisiteSkill = SkillType::COUNT; // 기본값 = 없음
	int requiredLevel = 0; // 위 스킬이 이 레벨 이상일 때만 해금 가능

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

	void Init() { g_playUI = nullptr; ReSetData(); }
	void UnInit();
	void ReSetData();
	bool CanUnlock(SkillType skill);
	bool CheckUnlock(SkillType skill) { return skillTree[skill].unlocked; }
	bool LevelUpSkill(SkillType skill);
	void AllSkillUnlock();
};