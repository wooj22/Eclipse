#include "Aron_Scene.h"
#include "EclipseApp.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include <random>
#include <algorithm>

void Aron_Scene::Awake()
{
	// [ camera ]
	cam = CreateObject<GameObject>();
	cam->AddComponent<Transform>();
	auto camCompo = cam->AddComponent<Camera>(1920, 1080);

	// [ BackGround ]
	auto backGround = CreateObject<GameObject>();
	backGround->AddComponent<Transform>()->SetPosition(0.0f, 0.0f);
	auto background_sr = backGround->AddComponent<SpriteRenderer>();
	background_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Background.png"), "Background_Aron");

	// [ title ]
	title_text = CreateObject<UI_Text>();
	title_text->rectTransform->SetPosition(0, 500);
	title_text->rectTransform->SetSize(500, 100);
	title_text->screenTextRenderer->SetFontSize(50);
	title_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::LightPink));
	title_text->screenTextRenderer->SetText(L"Aron Scene");

	// [ score ]
	score_text = CreateObject<UI_Text>();
	score_text->rectTransform->SetPosition(-800, 450);
	score_text->rectTransform->SetSize(300, 50);
	score_text->screenTextRenderer->SetFontSize(30);
	score_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	score_text->screenTextRenderer->SetText(L"Score: 0");

	// [ debug ]
	debug_text = CreateObject<UI_Text>();
	debug_text->rectTransform->SetPosition(-800, 400);
	debug_text->rectTransform->SetSize(300, 50);
	debug_text->screenTextRenderer->SetFontSize(20);
	debug_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));
	debug_text->screenTextRenderer->SetText(L"Debug Info");

	// [ player ] 
	player = CreateObject<Player>();

	// [ playerAttack_Parent ]
	playerAttack_Parent = CreateObject<GameObject>();
	auto playerAttack_Parent_tr = playerAttack_Parent->AddComponent<Transform>();
	playerAttack_Parent_tr->SetParent(player->transform);
	playerAttack_Parent_tr->SetPosition(0.0f, 0.0f);
	player->playerFSM->SetPlayerAttackParent(playerAttack_Parent);

	// [ playerAttack ] Attack 이펙트 & 콜라이더 설정 
	playerAttackArea = CreateObject<PlayerAttackArea>();
	playerAttackArea->GetComponent<Transform>()->SetParent(playerAttack_Parent->transform);
	playerAttackArea->SetActive(false);
	player->playerFSM->SetPlayerAttackArea(playerAttackArea); // 플레이어 FSM에 설정

	// [ ground ] - 메인 바닥 (긴 콜라이더 - 혼문 제거용)
	ground = CreateObject<GameObject>();
	ground->name = "Ground";
	ground->AddComponent<Transform>()->SetPosition(0.0f, -350.0f);

	auto ground_sr = ground->AddComponent<SpriteRenderer>();
	ground_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Ground.png"), "Ground");
	ground_sr->layer = 0;

	ground_col = ground->AddComponent<BoxCollider>();
	ground_col->size = { 2000.0f, 30.0f }; // 긴 콜라이더 (혼문 제거용)
	ground_col->isTrigger = true;

	// floor2 제거 - 문씬에는 없는 중간층이므로 삭제

	// [ playerGround ] - 플레이어용 바닥 (물리 충돌)
	playerGround = CreateObject<GameObject>();
	playerGround->name = "Ground";
	playerGround->AddComponent<Transform>()->SetPosition(0.0f, -350.0f);
	playerGround_col = playerGround->AddComponent<BoxCollider>();
	playerGround_col->size = { 1110.0f, 30.0f };

	// [ wall_r ]
	wall_r = CreateObject<GameObject>();
	wall_r->name = "Wall";
	wall_r->AddComponent<Transform>()->SetPosition(550.0f, 0.0f);

	auto wall_r_sr = wall_r->AddComponent<SpriteRenderer>();
	wall_r_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Wall.png"), "Wall");

	wall_r_col = wall_r->AddComponent<BoxCollider>();
	wall_r_col->size = { 30.0f, 750.0f };

	// [ wall_l ]
	wall_l = CreateObject<GameObject>();
	wall_l->name = "Wall";
	wall_l->AddComponent<Transform>()->SetPosition(-550.0f, 0.0f);

	auto wall_l_sr = wall_l->AddComponent<SpriteRenderer>();
	wall_l_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Wall.png"), "Wall");

	wall_l_col = wall_l->AddComponent<BoxCollider>();
	wall_l_col->size = { 30.0f, 750.0f };

	// [ Platform1 ]
	platform1 = CreateObject<GameObject>();
	platform1->name = "Ground";
	platform1->AddComponent<Transform>()->SetPosition(-300.0f, -200.0f);

	auto platform1_sr = platform1->AddComponent<SpriteRenderer>();
	platform1_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Platform.png"), "Platform");

	platform1_col = platform1->AddComponent<BoxCollider>();
	platform1_col->offset = { 0.0f, 12.0f };
	platform1_col->size = { 200.0f, 5.0f };

	// [ Platform2 ]
	platform2 = CreateObject<GameObject>();
	platform2->name = "Ground";
	platform2->AddComponent<Transform>()->SetPosition(200.0f, 0.0f);

	auto platform2_sr = platform2->AddComponent<SpriteRenderer>();
	platform2_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Platform.png"), "Platform");

	platform2_col = platform2->AddComponent<BoxCollider>();
	platform2_col->size = { 200.0f, 30.0f };

	// boundary condition
	Rect mapRect;
	mapRect.size = { 2560, 1920 };

	// camera target
	camCompo->SetTarget(player->transform);
	camCompo->SetTargetTraceSpeed(200.0f);
	camCompo->SetTargetTraceLimitX(30.0f);
	camCompo->SetTargetTraceLimitY(100.0f);
	camCompo->SetMapCondition(mapRect);

	// 웨이브 시스템 초기화 - 각 웨이브별 혼문 타입 관리
	waveData.currentWave = 1;
	waveData.waveActive = false;
	waveData.totalSpawnCount = 20;  // 웨이브당 20개
	waveData.currentSpawnIndex = 0;
	waveData.spawnInterval = 1.0f;
	waveData.lastSpawnTime = 0.0f;
	waveData.spawnY = 800.0f;  // 혼문 스폰 위치를 낮게 조정
	waveData.groundY = -350.0f;
	waveData.floor2Y = -180.0f;
	waveData.waveDuration = 60.0f;  // 웨이브 지속 시간 (60초)

	// 혼문 관리 시스템 초기화
	honmunManager.targetCount = 20;
	honmunManager.currentCount = 0;
	honmunManager.mapBoundaryX = 1280.0f;
	honmunManager.mapBoundaryY = 720.0f;
	honmunManager.respawnInterval = 0.5f;
	honmunManager.lastRespawnTime = 0.0f;
	honmunManager.isUpdating = false;

	// 점수 시스템 초기화
	currentScore = 0;
	
	// 혼의 개수 시스템 초기화
	soulCount = 10;  // 시작 혼의 개수
}

void Aron_Scene::Start()
{
	// game object -> SceneStart()
	__super::Start();

	// 웨이브 1 시작
	StartWave1();
}

void Aron_Scene::Update()
{
	// game object -> Update()
	__super::Update();

	// scene change
	if (Input::GetKeyDown('1'))
	{
		SceneManager::Get().ChangeScene(EclipseApp::MOON);
	}
	if (Input::GetKeyDown('2'))
	{
		SceneManager::Get().ChangeScene(EclipseApp::YUNMO);
	}
	if (Input::GetKeyDown('3'))
	{
		SceneManager::Get().ChangeScene(EclipseApp::ARON);
	}
	if (Input::GetKeyDown('4'))
	{
		SceneManager::Get().ChangeScene(EclipseApp::WOO);
	}
	if (Input::GetKeyDown('5'))
	{
		SceneManager::Get().ChangeScene(EclipseApp::MENU);
	}
	if (Input::GetKeyDown('6'))
	{
		SceneManager::Get().ChangeScene(EclipseApp::PLAY);
	}
	if (Input::GetKeyDown('7'))
	{
		SceneManager::Get().ChangeScene(EclipseApp::END);
	}

	// 웨이브 시스템 업데이트
	UpdateWaveSystem();

	// UI 업데이트
	UpdateScoreUI();

	// AABB 그리기 
	ground_col->DebugColliderDraw();
	wall_r_col->DebugColliderDraw();
	wall_l_col->DebugColliderDraw();
	platform1_col->DebugColliderDraw();
	platform2_col->DebugColliderDraw();
	// floor2_col 제거됨
	if (playerGround_col) playerGround_col->DebugColliderDraw();
}

void Aron_Scene::Exit()
{
	// game object -> destroy()
	__super::Exit();
}

// 웨이브 시스템 함수들
void Aron_Scene::StartWave1()
{
	waveData.currentWave = 1;
	waveData.waveActive = true;
	waveData.currentSpawnIndex = 0;
	waveData.waveStartTime = Time::GetTotalTime();
	waveData.spawnedHonmuns.clear();
	
	// 웨이브 1: A, B, C 타입 스폰
	char waveMsg[100];
	sprintf_s(waveMsg, "=== WAVE 1 STARTED === (A, B, C types - %d honmuns)\n", waveData.totalSpawnCount);
	OutputDebugStringA(waveMsg);
}

void Aron_Scene::StartWave2()
{
	waveData.currentWave = 2;
	waveData.waveActive = true;
	waveData.currentSpawnIndex = 0;
	waveData.waveStartTime = Time::GetTotalTime();
	waveData.spawnedHonmuns.clear();
	
	// 웨이브 2: A, B, C 타입 스폰
	char waveMsg[100];
	sprintf_s(waveMsg, "=== WAVE 2 STARTED === (A, B, C types - %d honmuns)\n", waveData.totalSpawnCount);
	OutputDebugStringA(waveMsg);
}

void Aron_Scene::StartWave3()
{
	waveData.currentWave = 3;
	waveData.waveActive = true;
	waveData.currentSpawnIndex = 0;
	waveData.waveStartTime = Time::GetTotalTime();
	waveData.spawnedHonmuns.clear();
	
	// 웨이브 3: A, B, C, D 타입 스폰
	char waveMsg[100];
	sprintf_s(waveMsg, "=== WAVE 3 STARTED === (A, B, C, D types - %d honmuns)\n", waveData.totalSpawnCount);
	OutputDebugStringA(waveMsg);
}

void Aron_Scene::UpdateWaveSystem()
{
	if (!waveData.waveActive) return;

	float currentTime = Time::GetTotalTime();

	// 스폰 시간 체크
	if (waveData.currentSpawnIndex < waveData.totalSpawnCount &&
		currentTime - waveData.lastSpawnTime >= waveData.spawnInterval)
	{
		// 웨이브에 따라 다른 스폰 함수 호출
		if (waveData.currentWave == 1)
		{
			SpawnHonmun();  // 웨이브 1: A, B만
		}
		else if (waveData.currentWave == 2)
		{
			SpawnHonmunWave2();  // 웨이브 2: A, B, C 포함
		}
		else if (waveData.currentWave == 3)
		{
			SpawnHonmunWave3();  // 웨이브 3: A, B, C, D 포함
		}

		waveData.lastSpawnTime = currentTime;
		waveData.currentSpawnIndex++;
	}

	// 웨이브 완료 체크
	if (waveData.currentSpawnIndex >= waveData.totalSpawnCount)
	{
		float waveDuration = currentTime - waveData.waveStartTime;
		if (waveDuration >= waveData.waveDuration)
		{
			CompleteCurrentWave();
		}
	}
}

void Aron_Scene::SpawnHonmun()
{
	// 랜덤 X 위치 (카메라 영역 내)
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> xDis(-1000.0f, 1000.0f);

	float spawnX = xDis(gen);

	// 새로운 혼문 생성 (Wave 1은 A, B, C 포함)
	auto* newHonmun = CreateObject<Honmun>();
	HonmunType spawnType = GetRandomHonmunTypeWave1();
	newHonmun->SetHonmunType(spawnType);
	newHonmun->SetPosition(spawnX, waveData.spawnY);
	
	// 스폰 타입 및 위치 상세 로그
	char spawnMsg[150];
	sprintf_s(spawnMsg, "🔵 Wave1 Spawned: %s (type:%d) at (%.1f, %.1f) - Ground at Y=-350\n", 
	         newHonmun->name.c_str(), static_cast<int>(spawnType), spawnX, waveData.spawnY);
	OutputDebugStringA(spawnMsg);
	
	sprintf_s(spawnMsg, "스폰 높이 분석: Y=%.1f, Ground까지 거리=%.1f (%.1f낙하 필요)\n", 
	         waveData.spawnY, waveData.spawnY - (-350.0f), waveData.spawnY + 350.0f);
	OutputDebugStringA(spawnMsg);

	// 관리 리스트에 추가
	waveData.spawnedHonmuns.push_back(newHonmun);
	honmunManager.activeHonmuns.push_back(newHonmun);
	honmunManager.currentCount++;

	char debugMsg[100];
	sprintf_s(debugMsg, "Spawned Honmun Wave1 %d/%d at (%.1f, %.1f)\n",
		waveData.currentSpawnIndex + 1, waveData.totalSpawnCount, spawnX, waveData.spawnY);
	OutputDebugStringA(debugMsg);
}

void Aron_Scene::SpawnHonmunWave2()
{
	// 랜덤 X 위치 (카메라 영역 내)
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> xDis(-1000.0f, 1000.0f);

	float spawnX = xDis(gen);

	// 새로운 혼문 생성 (Wave 2는 A, B, C 타입 포함)
	auto* newHonmun = CreateObject<Honmun>();
	newHonmun->SetHonmunType(GetRandomHonmunTypeWave2());
	newHonmun->SetPosition(spawnX, waveData.spawnY);

	// 관리 리스트에 추가
	waveData.spawnedHonmuns.push_back(newHonmun);
	honmunManager.activeHonmuns.push_back(newHonmun);
	honmunManager.currentCount++;

	char debugMsg[100];
	sprintf_s(debugMsg, "Spawned Honmun Wave2 %d/%d at (%.1f, %.1f)\n",
		waveData.currentSpawnIndex + 1, waveData.totalSpawnCount, spawnX, waveData.spawnY);
	OutputDebugStringA(debugMsg);
}

void Aron_Scene::SpawnHonmunWave3()
{
	// 랜덤 X 위치 (카메라 영역 내)
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> xDis(-1000.0f, 1000.0f);

	float spawnX = xDis(gen);

	// 새로운 혼문 생성 (Wave 3는 A, B, C, D 타입 포함)
	auto* newHonmun = CreateObject<Honmun>();
	newHonmun->SetHonmunType(GetRandomHonmunTypeWave3());
	newHonmun->SetPosition(spawnX, waveData.spawnY);

	// 관리 리스트에 추가
	waveData.spawnedHonmuns.push_back(newHonmun);
	honmunManager.activeHonmuns.push_back(newHonmun);
	honmunManager.currentCount++;

	char debugMsg[100];
	sprintf_s(debugMsg, "Spawned Honmun Wave3 %d/%d at (%.1f, %.1f)\n",
		waveData.currentSpawnIndex + 1, waveData.totalSpawnCount, spawnX, waveData.spawnY);
	OutputDebugStringA(debugMsg);
}

HonmunType Aron_Scene::GetRandomHonmunTypeWave1()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dis(0, 2); // C 타입도 포함

	int choice = dis(gen);
	if (choice == 0) return HonmunType::A;
	else if (choice == 1) return HonmunType::B;
	else return HonmunType::C; // Wave 1에서도 C 타입 스폰
}

HonmunType Aron_Scene::GetRandomHonmunTypeWave2()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dis(0, 2);

	int choice = dis(gen);
	if (choice == 0) return HonmunType::A;
	else if (choice == 1) return HonmunType::B;
	else return HonmunType::C;
}

HonmunType Aron_Scene::GetRandomHonmunTypeWave3()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dis(0, 3);

	int choice = dis(gen);
	if (choice == 0) return HonmunType::A;
	else if (choice == 1) return HonmunType::B;
	else if (choice == 2) return HonmunType::C;
	else return HonmunType::D;
}

void Aron_Scene::CompleteCurrentWave()
{
	waveData.waveActive = false;
	waveData.waveCompleted = true;

	char waveMsg[100];
	sprintf_s(waveMsg, "Wave %d completed!\n", waveData.currentWave);
	OutputDebugStringA(waveMsg);

	// 다음 웨이브 시작
	if (waveData.currentWave < 3)
	{
		StartNextWave();
	}
}

void Aron_Scene::StartNextWave()
{
	if (waveData.currentWave == 1)
	{
		StartWave2();
	}
	else if (waveData.currentWave == 2)
	{
		StartWave3();
	}
}

void Aron_Scene::AddScore(int points)
{
	currentScore += points;
}

void Aron_Scene::DecreaseSoulCount()
{
	if (soulCount > 0)
	{
		soulCount--;
		char soulMsg[100];
		sprintf_s(soulMsg, "Soul fell to ground! Soul count decreased: %d\n", soulCount);
		OutputDebugStringA(soulMsg);
	}
	else
	{
		OutputDebugStringA("Soul count already at 0, cannot decrease further\n");
	}
}

void Aron_Scene::UpdateScoreUI()
{
	if (score_text)
	{
		wchar_t scoreBuffer[50];
		swprintf_s(scoreBuffer, L"Score: %d", currentScore);
		score_text->screenTextRenderer->SetText(scoreBuffer);
	}

	if (debug_text)
	{
		wchar_t debugBuffer[150];
		swprintf_s(debugBuffer, L"Wave: %d | Honmuns: %d | Souls: %d", 
			waveData.currentWave, honmunManager.currentCount, soulCount);
		debug_text->screenTextRenderer->SetText(debugBuffer);
	}
}

// 호환성을 위한 혼문 관리 함수들
void Aron_Scene::AddHonmunToManager(Honmun* honmun)
{
	if (honmun)
	{
		honmunManager.activeHonmuns.push_back(honmun);
		honmunManager.currentCount++;
		
		char debugMsg[100];
		sprintf_s(debugMsg, "Added honmun to manager: %s (Count: %d)\n", 
			honmun->name.c_str(), honmunManager.currentCount);
		OutputDebugStringA(debugMsg);
	}
}

void Aron_Scene::RemoveHonmunFromManager(Honmun* honmun)
{
	if (honmun)
	{
		auto it = std::find(honmunManager.activeHonmuns.begin(), honmunManager.activeHonmuns.end(), honmun);
		if (it != honmunManager.activeHonmuns.end())
		{
			honmunManager.activeHonmuns.erase(it);
			honmunManager.currentCount--;
			
			char debugMsg[100];
			sprintf_s(debugMsg, "Removed honmun from manager: %s (Count: %d)\n", 
				honmun->name.c_str(), honmunManager.currentCount);
			OutputDebugStringA(debugMsg);
		}
	}
}