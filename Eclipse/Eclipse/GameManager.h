#pragma once
#include "../Direct2D_EngineLib/Singleton.h"
#include <unordered_map>
#include <string>  
#include <vector>  

class PlayUI;

enum class ChatCondition { None, Success, Fail };

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

	COUNT				// 총 개수 관리용
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

//UI 스킬 해금 정보
struct SkillInfo {
	bool unlocked = false;  // 해금 여부
	int unlockLevel = 0;    // 해금된 레벨 (혹은 조건 레벨)
	int maxLevel = 1;		// 스킬 최대 레벨 (기본 1)

	// 선행 조건: optional
	SkillType prerequisiteSkill = SkillType::COUNT; // 기본값 = 없음
	int requiredLevel = 0; // 위 스킬이 이 레벨 이상일 때만 해금 가능
	std::vector<int> skillCost;

	SkillInfo(
		bool unlocked = false,
		int unlockLevel = 0,
		int maxLevel = 1,
		SkillType prerequisiteSkill = SkillType::COUNT,
		int requiredLevel = 0,
		const std::vector<int>& cost = {})
		: unlocked(unlocked),
		unlockLevel(unlockLevel),
		maxLevel(maxLevel),
		prerequisiteSkill(prerequisiteSkill),
		requiredLevel(requiredLevel),
		skillCost(cost)
	{
	}
};

//UI 스킬 설명 정보
struct SkillText {
	std::wstring skillname = L"";				// 스킬 이름
	std::wstring skillDesc = L"";				// 스킬 설명

	SkillText(
		
		std::wstring name = L"",
		std::wstring descriptions = L"")
		: 
		skillname(name),
		skillDesc(descriptions)
	{
	}
};

class GameManager : public Singleton<GameManager>
{

public:
	PlayUI* g_playUI;
	int honCount;

	// 마우스
	bool canUseMouse;

	//웨이브
	int waveCount;				// UI 에서 넘겨 줄 웨이브 카운트
	bool isWave;				// UI 에서 넘겨 줄 웨이브 시작 여부
	float waveTime;				// 웨이브에서 넘겨줄 웨이브 타임

	//퀘스트
	int questCount;
	ChatCondition questState;

	//플레이어 스킬
	float absorbCoolTime;		// Q 흡수 쿨타임				PlayerFSM -> absorbCooldownTimer
	bool canUseAbsorb;			// Q 흡수 스킬 사용 가능 여부	PlayerFSM -> isAbsorbSkillActive
	bool canUseRelease ;		// E 방출 스킬 사용 가능 여부	PlayerFSM -> isReleaseSkillAvailable

	std::unordered_map<SkillType, SkillInfo> skillTree;
	std::unordered_map<SkillType, std::vector<float>> skillValue;
	std::unordered_map<SkillType, SkillText> skillText;

	GameManager() = default;
	~GameManager() = default;

	void Init() { g_playUI = nullptr; ReSetData(); }
	void UnInit();
	void ReSetData();
	void WaveStart();

	// UI스킬 관련 함수
	void SkillReset();
	bool CanUnlock(SkillType skill);
	bool CheckUnlock(SkillType skill) { return skillTree[skill].unlocked; }
	bool LevelUpSkill(SkillType skill, bool check = false);
	void AllSkillUnlock();

	// 플레이어 스킬 관련 함수
	void CanAbsorb();
	void UseAbsorb();
	void CanRelease();
	void UseRelease();


	// 혼 관련 함수
	void ChangeHonCount(int num);

	// 웨이브 관련 함수
	void FinishWave();

	// 퀘스트 관련 함수
	void ChangeQuestCount(int waveidx);

	// 스킬 수치 값 
	float GetSkillBonus(SkillType type);

	// NPC 관련 함수 - 퀘스트 보상 및 대화창 오픈
	void OnNPCInteraction();

	// Boss 관련 함수
	void ChangeBossHp(float hp);
};