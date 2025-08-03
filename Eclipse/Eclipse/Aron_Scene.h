#pragma once
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/BoxCollider.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include <vector>
#include <random>
#include <unordered_map>
#include <memory>
#include <mutex>

// Forward declarations
class Honmun;
enum class HonmunType;
class Player;
class PlayerAttackArea;

class Aron_Scene : public Scene
{
public:
	// Scene��(��) ���� ��ӵ�
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void Exit() override;

private:
	// camera
	GameObject* cam = nullptr;

	// UI
	UI_Text* title_text = nullptr;
	UI_Text* score_text = nullptr;
	UI_Text* debug_text = nullptr;

	// 혼문 관리 시스템 (스마트 포인터 + unordered_map)
	std::unordered_map<int, std::shared_ptr<Honmun>> honmunRegistry;
	static inline int nextHonmunId = 0;
	int selectedHonmunId = -1;
	mutable std::mutex honmunMutex;
	
	// 임시 호환성을 위한 예전 변수들 (추후 제거 예정)
	Honmun* honmun_a = nullptr;
	Honmun* honmun_b = nullptr;
	std::vector<Honmun*> allHonmuns;
	int selectedHonmunIndex = 0;

	// 플레이어 시스템 (Moon_Scene에서 가져옴)
	Player* player = nullptr;						// 플레이어
	GameObject* playerAttack_Parent = nullptr;		// 공격 영역 부모
	PlayerAttackArea* playerAttackArea = nullptr;	// 공격 영역

	// ground for enemies to stand on
	GameObject* ground = nullptr;      // 1층 바닥 (파괴용)
	BoxCollider* ground_col = nullptr;
	GameObject* floor2 = nullptr;      // 2층 플랫폼 (통과 가능)
	BoxCollider* floor2_col = nullptr;
	
	// 플레이어용 구조물들 (Moon_Scene에서 가져옴)
	GameObject* playerGround = nullptr;   // 플레이어용 바닥
	BoxCollider* playerGround_col = nullptr;
	GameObject* wall_r = nullptr;        // 오른쪽 벽
	BoxCollider* wall_r_col = nullptr;
	GameObject* wall_l = nullptr;        // 왼쪽 벽
	BoxCollider* wall_l_col = nullptr;
	GameObject* platform1 = nullptr;     // 플랫폼 1
	BoxCollider* platform1_col = nullptr;
	GameObject* platform2 = nullptr;     // 플랫폼 2
	BoxCollider* platform2_col = nullptr;

	// 점수 시스템
	int currentScore = 0;
	
	// 웨이브 시스템
	struct WaveSpawnData {
		float spawnY = 1200.0f;         // 카메라 훨씬 위 스폰 위치 (더 넓게)
		float groundY = -350.0f;        // 바닥 위치 (1층 위쪽) - 충돌 테스트용
		float floor2Y = -180.0f;        // 2층 위치
		int totalSpawnCount = 20;       // 총 스폰할 혼문 수
		int currentSpawnIndex = 0;      // 현재 스폰된 개수
		float spawnInterval = 1.0f;     // 스폰 간격 (초)
		float lastSpawnTime = 0.0f;     // 마지막 스폰 시간
		bool waveActive = false;        // 웨이브 활성 상태
		int currentWave = 1;            // 현재 웨이브 번호 (1, 2, 3)
		float waveStartTime = 0.0f;     // 웨이브 시작 시간
		float waveDuration = 70.0f;     // 웨이브 지속 시간 (70초)
		bool waveCompleted = false;     // 웨이브 완료 여부
		std::vector<int> spawnedHonmunIds; // 스폰된 혼문 ID들
		std::vector<Honmun*> spawnedHonmuns; // 임시 호환성
	} waveData;
	
	// 혼문 관리 시스템 (스레드 안전)
	struct HonmunManagerData {
		int targetCount = 20;
		int currentCount = 0;
		float mapBoundaryX = 1280.0f;
		float mapBoundaryY = 720.0f;
		float respawnInterval = 0.5f;
		float lastRespawnTime = 0.0f;
		std::vector<Honmun*> activeHonmuns; // 임시 호환성
		bool isUpdating = false; // 임시 호환성
	} honmunManager;
	
	// collision detection function
	void CheckCollisionAndChangeColor();
	void HandleHonmunMovement();
	void HandleCameraMovement();
	
public:
	// 점수 관련 함수들 (다른 클래스에서 접근 가능)
	void AddScore(int points);
	int GetScore() const { return currentScore; }
	void UpdateScoreUI();
	
	// 웨이브 시스템 함수들
	void StartWave1();
	void StartWave2();
	void StartWave3();
	void UpdateWaveSystem();
	void SpawnHonmun();
	void SpawnHonmunWave2();  // 웨이브 2용 스폰 (A, B, C 포함)
	void SpawnHonmunWave3();  // 웨이브 3용 스폰 (A, B, C, D 포함)
	bool IsHonmunOnGround(Honmun* honmun);
	void CheckHonmunsReachFloor1();
	HonmunType GetRandomHonmunType();
	HonmunType GetRandomHonmunTypeWave1();  // 웨이브 1용 (A, B 만)
	HonmunType GetRandomHonmunTypeWave2();  // 웨이브 2용 (A, B, C 포함)
	HonmunType GetRandomHonmunTypeWave3();  // 웨이브 3용 (A, B, C, D 포함)
	void ResetScene();  // 씬 완전 초기화
	
	// 혼문 관리 시스템 함수들
	void UpdateHonmunManager();        // 혼문 수 관리 업데이트
	void CheckAndRemoveOutOfBounds();  // 맵 밖 혼문 제거
	void SpawnNewHonmun();             // 새 혼문 생성
	int CreateHonmun(HonmunType type, float x, float y);  // 안전한 혼문 생성
	bool RemoveHonmun(int id);                            // 안전한 혼문 제거
	std::shared_ptr<Honmun> GetHonmun(int id) const;     // 안전한 혼문 접근
	bool IsOutOfBounds(std::shared_ptr<Honmun> honmun); // 맵 밖 여부 확인
	
	// 임시 호환성 함수들
	void AddHonmunToManager(Honmun* honmun);
	void RemoveHonmunFromManager(Honmun* honmun);
	bool IsOutOfBounds(Honmun* honmun);
	
	// 웨이브 시스템 헬퍼 함수들
	bool AllEnemiesDefeated();
	void CompleteCurrentWave();
	void StartNextWave();
	
	// 안전한 메모리 관리 함수들
	void SafeCleanupDestroyedObjects();
	bool IsValidHonmun(Honmun* honmun);
};