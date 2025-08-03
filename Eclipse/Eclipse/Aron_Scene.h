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
#include <map>

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

	// \ud63c\ubb38 enemies (Honmun \ud074\ub798\uc2a4 \uc0ac\uc6a9) - \ubaa8\ub4e0 \ud0c0\uc785 \uc9c0\uc6d0
	Honmun* honmun_a = nullptr;
	Honmun* honmun_b = nullptr;
	Honmun* honmun_c = nullptr;  // C \ud0c0\uc785 \ud65c\uc131\ud654
	Honmun* honmun_c2 = nullptr; // C \ud0c0\uc785 2
	Honmun* honmun_c3 = nullptr; // C \ud0c0\uc785 3
	Honmun* honmun_c4 = nullptr; // C \ud0c0\uc785 4
	Honmun* honmun_d = nullptr;  // D \ud0c0\uc785 \ud65c\uc131\ud654
	Honmun* honmun_d2 = nullptr; // D \ud0c0\uc785 2
	Honmun* honmun_d3 = nullptr; // D \ud0c0\uc785 3
	Honmun* honmun_d4 = nullptr; // D \ud0c0\uc785 4
	
	// 오브젝트 선택 시스템
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
		std::vector<Honmun*> spawnedHonmuns; // 스폰된 혼문들
	} waveData;
	
	// 혼문 관리 시스템
	struct HonmunManagerData {
		int targetCount = 20;           // 목표 혼문 수 (항상 20개 유지)
		int currentCount = 0;           // 현재 화면의 혼문 수
		float mapBoundaryX = 1280.0f;   // 맵 X 경계 (고정 크기)
		float mapBoundaryY = 720.0f;    // 맵 Y 경계 (고정 크기)
		float respawnInterval = 0.5f;   // 혼문 보충 간격 (초)
		float lastRespawnTime = 0.0f;   // 마지막 보충 시간
		std::vector<Honmun*> activeHonmuns; // 활성 혼문 리스트
		bool isUpdating = false;        // 업데이트 중 플래그 (동시 수정 방지)
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
	void AddHonmunToManager(Honmun* honmun);    // 관리 리스트에 추가
	void RemoveHonmunFromManager(Honmun* honmun); // 관리 리스트에서 제거
	bool IsOutOfBounds(Honmun* honmun); // 맵 밖 여부 확인
};