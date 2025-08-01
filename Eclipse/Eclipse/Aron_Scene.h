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

// Forward declarations
class Honmun;
enum class HonmunType;

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

	// \ud63c\ubb38 enemies (Honmun \ud074\ub798\uc2a4 \uc0ac\uc6a9) - \uc6e8\uc774\ube0c 1 \ud14c\uc2a4\ud2b8: A, B\ub9cc \uc0ac\uc6a9
	Honmun* honmun_a = nullptr;
	Honmun* honmun_b = nullptr;
	// Honmun* honmun_c = nullptr;  // \uc6e8\uc774\ube0c 1 \ud14c\uc2a4\ud2b8\uc5d0\uc11c \uc8fc\uc11d\ucc98\ub9ac
	// Honmun* honmun_d = nullptr;  // \uc6e8\uc774\ube0c 1 \ud14c\uc2a4\ud2b8\uc5d0\uc11c \uc8fc\uc11d\ucc98\ub9ac
	
	// 오브젝트 선택 시스템
	std::vector<Honmun*> allHonmuns;
	int selectedHonmunIndex = 0;

	// ground for enemies to stand on
	GameObject* ground = nullptr;      // 1층 바닥 (파괴용)
	BoxCollider* ground_col = nullptr;
	GameObject* floor2 = nullptr;      // 2층 플랫폼 (통과 가능)
	BoxCollider* floor2_col = nullptr;

	// 점수 시스템
	int currentScore = 0;
	
	// 웨이브 1 시스템
	struct WaveSpawnData {
		float spawnY = 800.0f;          // 카메라 위 스폰 위치
		float groundY = -350.0f;        // 바닥 위치 (1층 위쪽)
		float floor2Y = -180.0f;        // 2층 위치
		int totalSpawnCount = 20;       // 총 스폰할 혼문 수
		int currentSpawnIndex = 0;      // 현재 스폰된 개수
		float spawnInterval = 1.0f;     // 스폰 간격 (초)
		float lastSpawnTime = 0.0f;     // 마지막 스폰 시간
		bool waveActive = false;        // 웨이브 활성 상태
		std::vector<Honmun*> spawnedHonmuns; // 스폰된 혼문들
	} waveData;
	
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
	void UpdateWaveSystem();
	void SpawnHonmun();
	bool IsHonmunOnGround(Honmun* honmun);
	void CheckHonmunsReachFloor1();
	HonmunType GetRandomHonmunType();
	void ResetScene();  // 씬 완전 초기화
};