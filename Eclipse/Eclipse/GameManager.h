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
	COUNT // 총 개수 관리용
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

	void Init() { ReSetData(); }
	void UnInit();
	void ReSetData();
	bool CanUnlock(SkillType skill);
	bool LevelUpSkill(SkillType skill);
};