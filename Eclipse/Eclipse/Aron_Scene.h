#pragma once
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/BoxCollider.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include <vector>
#include <random>

#include "Player.h"
#include "playerAttackArea.h"
#include "Honmun.h"

// Forward declarations
enum class HonmunType;

class Aron_Scene : public Scene
{
private:
	// 기본 오브젝트들 (Moon_Scene과 동일)
	GameObject* cam = nullptr;
	UI_Text* title_text = nullptr;
	UI_Text* score_text = nullptr;
	UI_Text* debug_text = nullptr;

	// 플레이어 시스템 (Moon_Scene과 동일)
	Player* player = nullptr;						// Parent
	GameObject* playerAttack_Parent = nullptr;		// Parent - Child
	PlayerAttackArea* playerAttackArea = nullptr;	// Parent - Child - Child 

	// 구조물들 (Moon_Scene과 동일)
	GameObject* ground = nullptr;      // 메인 바닥 (긴 콜라이더)
	// floor2 제거
	GameObject* playerGround = nullptr; // 플레이어용 바닥
	GameObject* wall_r = nullptr; 
	GameObject* wall_l = nullptr;
	GameObject* platform1 = nullptr;
	GameObject* platform2 = nullptr;

	// 콜라이더들
	BoxCollider* ground_col = nullptr;
	// floor2_col 제거
	BoxCollider* playerGround_col = nullptr;
	BoxCollider* wall_r_col = nullptr;
	BoxCollider* wall_l_col = nullptr;
	BoxCollider* platform1_col = nullptr;
	BoxCollider* platform2_col = nullptr;

	// 점수 시스템
	int currentScore = 0;
	
	// 혼의 개수 시스템
	int soulCount = 10;  // 시작 혼의 개수 (기본값)
	
	// 웨이브 시스템 (간단한 구조)
	struct WaveData {
		int currentWave = 1;
		bool waveActive = false;
		int totalSpawnCount = 20;
		int currentSpawnIndex = 0;
		float spawnInterval = 1.0f;
		float lastSpawnTime = 0.0f;
		float spawnY = 1800.0f;
		float groundY = -350.0f;
		float floor2Y = -180.0f;
		float waveStartTime = 0.0f;
		float waveDuration = 70.0f;
		bool waveCompleted = false;
		std::vector<Honmun*> spawnedHonmuns;
	} waveData;
	
	// 혼문 관리 시스템 (간단한 구조)
	struct HonmunManager {
		int targetCount = 20;
		int currentCount = 0;
		float mapBoundaryX = 1280.0f;
		float mapBoundaryY = 720.0f;
		float respawnInterval = 0.5f;
		float lastRespawnTime = 0.0f;
		std::vector<Honmun*> activeHonmuns;
		bool isUpdating = false;
	} honmunManager;

public:
	Aron_Scene() = default;
	~Aron_Scene() = default;

	// Scene 기본 함수들 (Moon_Scene과 동일)
	void Awake() override;
	void Start() override;
	void Update() override;
	void Exit() override;

	// 점수 시스템
	void AddScore(int points);
	int GetScore() const { return currentScore; }
	void UpdateScoreUI();
	
	// 혼의 개수 시스템
	void DecreaseSoulCount();  // 혼의 개수 -1 (지면 충돌 시)
	int GetSoulCount() const { return soulCount; }
	void SetSoulCount(int count) { soulCount = count; }
	
	// 웨이브 시스템 함수들
	void StartWave1();
	void StartWave2();
	void StartWave3();
	void UpdateWaveSystem();
	void SpawnHonmun();
	void SpawnHonmunWave2();
	void SpawnHonmunWave3();
	HonmunType GetRandomHonmunTypeWave1();
	HonmunType GetRandomHonmunTypeWave2();
	HonmunType GetRandomHonmunTypeWave3();
	void CompleteCurrentWave();
	void StartNextWave();
	
	// 웨이브 상태 조회
	bool IsWaveActive() const { return waveData.waveActive; }
	
	// 혼문 관리 함수들 (호환성을 위해 추가)
	void AddHonmunToManager(Honmun* honmun);
	void RemoveHonmunFromManager(Honmun* honmun);
};