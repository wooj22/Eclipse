#include "Aron_Scene.h"
#include "EclipseApp.h"
#include "Honmun.h"  // Honmun 클래스 포함
#include "GameManager.h"  // GameManager 추가
#include "Player.h"      // 플레이어 추가
#include "playerAttackArea.h"  // 공격 영역 추가
#include "Attack_State.h"  // 공격 상태 추가
#include "HonmunKnockbackTest.h" // 넉백 테스트 스크립트
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Time.h"

void Aron_Scene::Awake()
{
	// 임시 호환성 초기화
	honmun_a = nullptr;
	honmun_b = nullptr;
	selectedHonmunIndex = 0;
	allHonmuns.clear();
	
	// camera init - Moon_Scene 방식으로 변경
	cam = CreateObject<GameObject>();
	cam->AddComponent<Transform>();
	auto camCompo = cam->AddComponent<Camera>(1920, 1080);
	
	// boundary condition (고정 맵 크기)
	Rect mapRect;
	mapRect.size = { 2560, 1920 };
	
	// camera target 설정 (넓은 맵 탐험을 위해 플레이어 추적)
	// 플레이어 생성 후 SetTarget 호출 필요 - Start()에서 처리

	// create gameobject
	// title sample
	title_text = CreateObject<UI_Text>();
	title_text->rectTransform->SetPosition(0, 500);
	title_text->rectTransform->SetSize(500, 100);
	title_text->screenTextRenderer->SetFontSize(50);
	title_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::LightBlue));
	title_text->screenTextRenderer->SetText(L"Aron Scene");
	
	// 점수 UI 추가
	score_text = CreateObject<UI_Text>();
	score_text->rectTransform->SetPosition(0, 400);
	score_text->rectTransform->SetSize(300, 50);
	score_text->screenTextRenderer->SetFontSize(30);
	score_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));
	score_text->screenTextRenderer->SetText(L"Score: 0");
	
	// 디버그 정보 UI 추가
	debug_text = CreateObject<UI_Text>();
	debug_text->rectTransform->SetPosition(0, 300);
	debug_text->rectTransform->SetSize(600, 50);
	debug_text->screenTextRenderer->SetFontSize(20);
	debug_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	debug_text->screenTextRenderer->SetText(L"Q: Wave 1 | E: Wave 2 | T: Wave 3 | Ctrl+UHJK: Camera | C: Reset Cam | SPACE: Alpha | ZX: Select | R: Reset");

	// [ \ud63c\ubb38 enemies ] - \uc6e8\uc774\ube0c 1 \ud14c\uc2a4\ud2b8\uc6a9 A, B \uc544\uc774\ud15c \uc5ec\ub7ec \uac1c \uc0dd\uc131
	// A \ud0c0\uc785 \ud63c\ubb38 4\uac1c
	honmun_a = CreateObject<Honmun>();
	honmun_a->name = "Honmun_A1";
	honmun_a->SetHonmunType(HonmunType::A);
	honmun_a->SetPosition(-300.0f, 200.0f);

	auto* honmun_a2 = CreateObject<Honmun>();
	honmun_a2->name = "Honmun_A2";
	honmun_a2->SetHonmunType(HonmunType::A);
	honmun_a2->SetPosition(-200.0f, 300.0f);

	auto* honmun_a3 = CreateObject<Honmun>();
	honmun_a3->name = "Honmun_A3";
	honmun_a3->SetHonmunType(HonmunType::A);
	honmun_a3->SetPosition(-400.0f, 100.0f);

	auto* honmun_a4 = CreateObject<Honmun>();
	honmun_a4->name = "Honmun_A4";
	honmun_a4->SetHonmunType(HonmunType::A);
	honmun_a4->SetPosition(-100.0f, 150.0f);

	// B \ud0c0\uc785 \ud63c\ubb38 4\uac1c (더 넓게 배치)
	honmun_b = CreateObject<Honmun>();
	honmun_b->name = "Honmun_B1";
	honmun_b->SetHonmunType(HonmunType::B);
	honmun_b->SetPosition(150.0f, 250.0f);

	auto* honmun_b2 = CreateObject<Honmun>();
	honmun_b2->name = "Honmun_B2";
	honmun_b2->SetHonmunType(HonmunType::B);
	honmun_b2->SetPosition(350.0f, 400.0f);

	auto* honmun_b3 = CreateObject<Honmun>();
	honmun_b3->name = "Honmun_B3";
	honmun_b3->SetHonmunType(HonmunType::B);
	honmun_b3->SetPosition(450.0f, 50.0f);

	auto* honmun_b4 = CreateObject<Honmun>();
	honmun_b4->name = "Honmun_B4";
	honmun_b4->SetHonmunType(HonmunType::B);
	honmun_b4->SetPosition(50.0f, 50.0f);

	// C 타입 혼문 임시 제거 (테스트용)
	// auto* honmun_c = CreateObject<Honmun>();
	// honmun_c->name = "Honmun_C1";
	// honmun_c->SetHonmunType(HonmunType::C);
	// honmun_c->SetPosition(-500.0f, -200.0f);

	// auto* honmun_c2 = CreateObject<Honmun>();
	// honmun_c2->name = "Honmun_C2";
	// honmun_c2->SetHonmunType(HonmunType::C);
	// honmun_c2->SetPosition(500.0f, -200.0f);

	// 모든 혼문을 벡터에 추가 (C 타입 제외)
	allHonmuns.push_back(honmun_a);
	allHonmuns.push_back(honmun_a2);
	allHonmuns.push_back(honmun_a3);
	allHonmuns.push_back(honmun_a4);
	allHonmuns.push_back(honmun_b);
	allHonmuns.push_back(honmun_b2);
	allHonmuns.push_back(honmun_b3);
	allHonmuns.push_back(honmun_b4);
	// allHonmuns.push_back(honmun_c);   // 임시 제거
	// allHonmuns.push_back(honmun_c2);  // 임시 제거
	
	// 웨이브 시스템 초기화
	waveData.waveActive = false;
	waveData.currentSpawnIndex = 0;
	waveData.lastSpawnTime = 0.0f;
	waveData.spawnInterval = 1.0f;
	waveData.spawnedHonmuns.clear();
	currentScore = 0;
	
	// 선택 인덱스 안전 초기화
	selectedHonmunIndex = 0;

	// 웨이브 1 테스트: C, D 주석처리
	// honmun_c = CreateObject<Honmun>();
	// honmun_c->name = "Honmun_C";
	// honmun_c->SetHonmunType(HonmunType::C);
	// honmun_c->SetPosition(100.0f, 200.0f);

	// honmun_d = CreateObject<Honmun>();
	// honmun_d->name = "Honmun_D";
	// honmun_d->SetHonmunType(HonmunType::D);
	// honmun_d->SetPosition(300.0f, 200.0f);

	// [ ground system ] - 1층(파괴), 2층(통과) 구조
	// 1층 바닥 (혼문이 닿으면 파괴됨)
	ground = CreateObject<GameObject>();
	ground->name = "Ground_Floor1";
	ground->AddComponent<Transform>()->SetPosition(0.0f, -400.0f); // 더 아래로 이동
	auto ground_sr = ground->AddComponent<SpriteRenderer>();
	ground_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Ground.png"), "Ground");
	ground_sr->layer = 0;
	ground_col = ground->AddComponent<BoxCollider>();
	ground_col->size = { 1110.0f, 30.0f };
	ground_col->isTrigger = true; // 트리거로 설정 (파괴 감지용)
	auto ground_rb = ground->AddComponent<Rigidbody>();
	ground_rb->useGravity = false;
	ground_rb->isKinematic = true;
	// 1층 바닥은 트리거로 설정됨 (파괴 감지는 Update에서 처리)
	
	// 2층 플랫폼 제거됨 (3층 위 콜라이더 제거 요청)
	// floor2 = CreateObject<GameObject>();
	// floor2->name = "Ground_Floor2";
	// floor2->AddComponent<Transform>()->SetPosition(0.0f, -200.0f); // 중간 높이
	// auto floor2_sr = floor2->AddComponent<SpriteRenderer>();
	// floor2_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Ground.png"), "Ground");
	// floor2_sr->layer = 0;
	// floor2_col = floor2->AddComponent<BoxCollider>();
	// floor2_col->size = { 1110.0f, 20.0f }; // 조금 더 얇게
	// floor2_col->isTrigger = true; // 트리거로 설정 (통과 가능)
	// auto floor2_rb = floor2->AddComponent<Rigidbody>();
	// floor2_rb->useGravity = false;
	// floor2_rb->isKinematic = true;
	// 2층 플랫폼은 트리거로 설정됨 (통과 가능)
	
	// 변수 초기화 (제거된 객체)
	floor2 = nullptr;
	floor2_col = nullptr;

	// [ Player System ] - Moon_Scene 완전 복붙 (문선민님 허락)
	// [ player ] 
	player = CreateObject<Player>();

	// [ playerAttack_Parent ]
	playerAttack_Parent = CreateObject<GameObject>();
	auto playerAttack_Parent_tr = playerAttack_Parent->AddComponent<Transform>();
	playerAttack_Parent_tr->SetParent(player->transform);
	playerAttack_Parent_tr->SetPosition(0.0f, 0.0f);
	player->playerFSM->SetPlayerAttackParent(playerAttack_Parent);

	// [ playerAttack ] Attack 이펙트 & 콜라이더 영역 
	playerAttackArea = CreateObject<PlayerAttackArea>();
	playerAttackArea->GetComponent<Transform>()->SetParent(playerAttack_Parent->transform);
	playerAttackArea->SetActive(false);
	player->playerFSM->SetPlayerAttackArea(playerAttackArea); // 플레이어 FSM에 연결
	
	// 혼문 넉백을 위한 스크립트 추가
	auto* attackKnockbackScript = playerAttackArea->AddComponent<HonmunKnockbackTest>();

	// [ ground ] - Moon_Scene과 동일
	ground = CreateObject<GameObject>();
	ground->name = "Ground";
	ground->AddComponent<Transform>()->SetPosition(0.0f, -350.0f);

	auto moon_ground_sr = ground->AddComponent<SpriteRenderer>();
	moon_ground_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Ground.png"), "Ground");
	moon_ground_sr->layer = 0;

	ground_col = ground->AddComponent<BoxCollider>();
	ground_col->size = { 2000.0f, 30.0f }; // 충돌 연구를 위해 더욱 넓게

	// [ wall_r ] - Moon_Scene과 동일
	wall_r = CreateObject<GameObject>();
	wall_r->name = "Wall";
	wall_r->AddComponent<Transform>()->SetPosition(1200.0f, 0.0f); // 충돌 연구를 위해 더욱 멀리

	auto wall_r_sr = wall_r->AddComponent<SpriteRenderer>();
	wall_r_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Wall.png"), "Wall");

	wall_r_col = wall_r->AddComponent<BoxCollider>();
	wall_r_col->size = { 30.0f, 750.0f };

	// [ wall_l ] - Moon_Scene과 동일
	wall_l = CreateObject<GameObject>();
	wall_l->name = "Wall";
	wall_l->AddComponent<Transform>()->SetPosition(-1200.0f, 0.0f); // 충돌 연구를 위해 더욱 멀리

	auto wall_l_sr = wall_l->AddComponent<SpriteRenderer>();
	wall_l_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Wall.png"), "Wall");

	wall_l_col = wall_l->AddComponent<BoxCollider>();
	wall_l_col->size = { 30.0f, 750.0f };

	// [ Platform1 ] - Moon_Scene과 동일
	platform1 = CreateObject<GameObject>();
	platform1->name = "Ground";
	platform1->AddComponent<Transform>()->SetPosition(-600.0f, -200.0f); // 충돌 연구를 위해 더 넓게

	auto platform1_sr = platform1->AddComponent<SpriteRenderer>();
	platform1_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Platform.png"), "Platform");

	platform1_col = platform1->AddComponent<BoxCollider>();
	platform1_col->offset = { 0.0f, 12.0f };
	platform1_col->size = { 200.0f, 5.0f };

	// [ Platform2 ] - Moon_Scene과 동일
	platform2 = CreateObject<GameObject>();
	platform2->name = "Ground";
	platform2->AddComponent<Transform>()->SetPosition(500.0f, 0.0f); // 충돌 연구를 위해 더 넓게

	auto platform2_sr = platform2->AddComponent<SpriteRenderer>();
	platform2_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Platform.png"), "Platform");

	platform2_col = platform2->AddComponent<BoxCollider>();
	platform2_col->size = { 200.0f, 30.0f };
	
	// 혼문들 추가 (Moon_Scene 플레이어 시스템에 혼문 테스트 추가)
	// A 타입 혼문들 (플레이어 근처에 배치하여 테스트)
	honmun_a = CreateObject<Honmun>();
	honmun_a->name = "Honmun_A1";
	honmun_a->SetHonmunType(HonmunType::A);
	honmun_a->SetPosition(100.0f, -100.0f); // 플랫폼 근처
	// 물리 설정 (넉백 가능하도록)
	auto* honmunA_rb = honmun_a->GetComponent<Rigidbody>();
	if (honmunA_rb) {
		honmunA_rb->isKinematic = false;
		honmunA_rb->useGravity = false;
	}

	auto* test_honmun_a2 = CreateObject<Honmun>();
	test_honmun_a2->name = "Honmun_A2";
	test_honmun_a2->SetHonmunType(HonmunType::A);
	test_honmun_a2->SetPosition(-100.0f, -100.0f);
	// 물리 설정
	auto* honmunA2_rb = test_honmun_a2->GetComponent<Rigidbody>();
	if (honmunA2_rb) {
		honmunA2_rb->isKinematic = false;
		honmunA2_rb->useGravity = false;
	}

	// B 타입 혼문들
	honmun_b = CreateObject<Honmun>();
	honmun_b->name = "Honmun_B1";
	honmun_b->SetHonmunType(HonmunType::B);
	honmun_b->SetPosition(200.0f, 100.0f);
	// 물리 설정
	auto* honmunB_rb = honmun_b->GetComponent<Rigidbody>();
	if (honmunB_rb) {
		honmunB_rb->isKinematic = false;
		honmunB_rb->useGravity = false;
	}

	auto* test_honmun_b2 = CreateObject<Honmun>();
	test_honmun_b2->name = "Honmun_B2";
	test_honmun_b2->SetHonmunType(HonmunType::B);
	test_honmun_b2->SetPosition(-200.0f, 100.0f);
	// 물리 설정
	auto* honmunB2_rb = test_honmun_b2->GetComponent<Rigidbody>();
	if (honmunB2_rb) {
		honmunB2_rb->isKinematic = false;
		honmunB2_rb->useGravity = false;
	}

	// 혼문들을 벡터에 추가
	allHonmuns.push_back(honmun_a);
	allHonmuns.push_back(test_honmun_a2);
	allHonmuns.push_back(honmun_b);
	allHonmuns.push_back(test_honmun_b2);
	
	// 모든 혼문의 물리 설정을 강제로 다시 적용 (Awake에서 덮어쓰인 설정 복구)
	for (auto* honmun : allHonmuns) {
		auto* rb = honmun->GetComponent<Rigidbody>();
		if (rb) {
			rb->isKinematic = false;  // 충돌 감지를 위해 반드시 false
			rb->useGravity = false;   // 중력은 비활성화 유지
		}
		
		auto* collider = honmun->GetComponent<CircleCollider>();
		if (collider) {
			collider->isTrigger = true; // 트리거 모드 확실히 설정
		}
		
		// 혼문 관리 시스템에 등록
		AddHonmunToManager(honmun);
		
		OutputDebugStringA("Honmun physics reset for collision detection\n");
	}
	
	// 초기 혼문 관리 시스템 설정
	honmunManager.currentCount = static_cast<int>(honmunManager.activeHonmuns.size());
	OutputDebugStringA("Honmun manager initialized with test Honmuns\n");
}

// 임시 호환성 함수들 (기존 코드와의 호환성을 위해)
void Aron_Scene::AddHonmunToManager(Honmun* honmun)
{
    if (!honmun) return;
    honmunManager.activeHonmuns.push_back(honmun);
    honmunManager.currentCount = static_cast<int>(honmunManager.activeHonmuns.size());
}

void Aron_Scene::RemoveHonmunFromManager(Honmun* honmun)
{
    if (!honmun) return;
    auto it = std::find(honmunManager.activeHonmuns.begin(), honmunManager.activeHonmuns.end(), honmun);
    if (it != honmunManager.activeHonmuns.end()) {
        honmunManager.activeHonmuns.erase(it);
        honmunManager.currentCount = static_cast<int>(honmunManager.activeHonmuns.size());
    }
}

bool Aron_Scene::IsOutOfBounds(Honmun* honmun)
{
    // CRASH FIX: 모든 접근을 완전히 차단하고 항상 false 반환
    // 혼문 제거를 비활성화하여 크래시 방지
    return false;
}

//  NEW: 웨이브 시스템 헬퍼 함수들
bool Aron_Scene::AllEnemiesDefeated()
{
    // 화면에 살아있는 혼문이 있는지 체크
    for (auto* honmun : waveData.spawnedHonmuns)
    {
        if (honmun && honmun->IsActive())
        {
            return false; // 살아있는 혼문 발견
        }
    }
    return true; // 모든 혼문이 처치됨
}

void Aron_Scene::CompleteCurrentWave()
{
    char debugMsg[100];
    sprintf_s(debugMsg, "Wave %d completed!\n", waveData.currentWave);
    OutputDebugStringA(debugMsg);
    
    // 현재 웨이브 정리
    waveData.waveActive = false;
    waveData.waveCompleted = true;
    waveData.spawnedHonmuns.clear();
    
    // 자동으로 다음 웨이브 시작
    if (waveData.currentWave < 3)
    {
        // 다음 웨이브 즉시 시작
        sprintf_s(debugMsg, "Starting Wave %d immediately (70 seconds each)\n", waveData.currentWave + 1);
        OutputDebugStringA(debugMsg);
        
        // 즉시 다음 웨이브 시작 (각 웨이브 70초씩)
        StartNextWave();
    }
    else
    {
        OutputDebugStringA("All waves completed! Victory!\n");
        // 게임 클리어 처리
    }
}

void Aron_Scene::StartNextWave()
{
    int nextWave = waveData.currentWave + 1;
    
    //  FIXED: 시간 리셋 추가
    waveData.lastSpawnTime = Time::GetTotalTime(); // 현재 시간으로 리셋
    
    switch (nextWave)
    {
        case 2:
            StartWave2();
            break;
        case 3:
            StartWave3();
            break;
        default:
            OutputDebugStringA("No more waves available\n");
            break;
    }
}

//  NEW: 안전한 객체 정리 시스템
void Aron_Scene::SafeCleanupDestroyedObjects()
{
    std::vector<Honmun*> toDestroy;
    
    // 1. spawnedHonmuns에서 파괴된 객체들 제거
    for (auto it = waveData.spawnedHonmuns.begin(); it != waveData.spawnedHonmuns.end();)
    {
        auto* honmun = *it;
        if (!honmun || !honmun->IsActive())
        {
            if (honmun) toDestroy.push_back(honmun);
            it = waveData.spawnedHonmuns.erase(it);
        }
        else
        {
            ++it;
        }
    }
    
    // 2. allHonmuns에서도 제거
    for (auto it = allHonmuns.begin(); it != allHonmuns.end();)
    {
        auto* honmun = *it;
        if (!honmun || !honmun->IsActive())
        {
            it = allHonmuns.erase(it);
        }
        else
        {
            ++it;
        }
    }
    
    // 3. activeHonmuns에서도 제거
    for (auto it = honmunManager.activeHonmuns.begin(); it != honmunManager.activeHonmuns.end();)
    {
        auto* honmun = *it;
        if (!honmun || !honmun->IsActive())
        {
            it = honmunManager.activeHonmuns.erase(it);
        }
        else
        {
            ++it;
        }
    }
    
    // 4. 마지막에 안전하게 파괴
    for (auto* honmun : toDestroy)
    {
        if (honmun)
        {
            honmun->Destroy();  // 이제 안전하게 파괴
        }
    }
    
    if (!toDestroy.empty())
    {
        char debugMsg[100];
        sprintf_s(debugMsg, "SafeCleanup: Destroyed %zu objects safely\n", toDestroy.size());
        OutputDebugStringA(debugMsg);
    }
}

bool Aron_Scene::IsValidHonmun(Honmun* honmun)
{
    return honmun != nullptr && honmun->IsActive();
}

void Aron_Scene::Start()
{
	// game object -> SceneStart()
	__super::Start();
	
	// Moon Scene과 동일한 카메라 타겟 설정
	if (cam && player)
	{
		auto* camCompo = cam->GetComponent<Camera>();
		if (camCompo)
		{
			camCompo->SetTarget(player->transform);
			camCompo->SetTargetTraceSpeed(200.0f);
			camCompo->SetTargetTraceLimitX(30.0f);
			camCompo->SetTargetTraceLimitY(100.0f);
			
			// 확장된 맵 크기 설정
			Rect mapRect;
			mapRect.size = { 3200, 2400 }; // 충돌 연구를 위해 더 넓게
			camCompo->SetMapCondition(mapRect);
			
			OutputDebugStringA("Camera target set to player (Moon Scene style)\n");
		}
	}
}

void Aron_Scene::Update()
{
	// game object -> Update()
	__super::Update();

	// 충돌 테스트 및 색상 변경
	CheckCollisionAndChangeColor();
	
	// 혼문 이동 컨트롤
	HandleHonmunMovement();
	
	// 웨이브 시스템 업데이트
	UpdateWaveSystem();
	
	// 1층 바닥 도달 체크 (혼문 파괴)
	CheckHonmunsReachFloor1();
	
	// UI 업데이트
	UpdateScoreUI();
	
	// 혼문 관리 시스템 업데이트
	UpdateHonmunManager();

	// 마우스 클릭 공격 처리 (안전한 null 체크 추가)
	if (Input::GetMouseButtonDown(0)) // 왼쪽 마우스 버튼
	{
		OutputDebugStringA("Mouse left click detected\n");
		
		if (player && player->IsActive())
		{
			// 플레이어 공격 활성화
			auto* playerFSM = player->GetComponent<PlayerFSM>();
			if (playerFSM)
			{
				// MovementFSM을 통한 공격 상태 전환
				try {
					auto* movementFSM = playerFSM->GetMovementFSM();
					if (movementFSM)
					{
						movementFSM->ChangeState(std::make_unique<Attack_State>());
						OutputDebugStringA("Mouse click - Player attack triggered!\n");
					}
					else
					{
						OutputDebugStringA("MovementFSM is null\n");
					}
				} catch (...) {
					OutputDebugStringA("Exception in mouse click attack handling\n");
				}
			}
			else
			{
				OutputDebugStringA("PlayerFSM is null\n");
			}
		}
		else
		{
			OutputDebugStringA("Player is null or inactive\n");
		}
	}
	
	// 웨이브 1 시작 (Q키로 변경 - 스페이스바 충돌 방지) - 활성화되지 않은 경우에만
	if (Input::GetKeyDown('Q'))
	{
		if (!waveData.waveActive)
		{
			StartWave1();
		}
		else
		{
			OutputDebugStringA("Wave already active! Press R to reset first.\n");
		}
	}
	
	// 웨이브 2 시작 (E키로 변경 - 숫자키 충돌 방지)
	if (Input::GetKeyDown('E'))
	{
		if (!waveData.waveActive)
		{
			StartWave2();
		}
		else
		{
			OutputDebugStringA("Wave already active! Press R to reset first.\n");
		}
	}
	
	// 웨이브 3 시작 (T키)
	if (Input::GetKeyDown('T'))
	{
		if (!waveData.waveActive)
		{
			StartWave3();
		}
		else
		{
			OutputDebugStringA("Wave already active! Press R to reset first.\n");
		}
	}
	
	// R키로 씬 완전 초기화
	if (Input::GetKeyDown('R'))
	{
		ResetScene();
	}
	
	// 카메라 수동 조작 (Ctrl 키를 누르고 있을 때만)
	if (Input::GetKey(VK_CONTROL))
	{
		HandleCameraMovement();
	}

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
		// '3' 키로 Aron 씬 재시작 (완전 초기화)
		OutputDebugStringA("Restarting Aron Scene (key 3)...\n");
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

	// AABB 그리기 - Moon_Scene과 동일한 디버그 드로잉
	if (ground_col) ground_col->DebugColliderDraw();
	if (wall_r_col) wall_r_col->DebugColliderDraw();
	if (wall_l_col) wall_l_col->DebugColliderDraw();
	if (platform1_col) platform1_col->DebugColliderDraw();
	if (platform2_col) platform2_col->DebugColliderDraw();
	// if (floor2_col) floor2_col->DebugColliderDraw(); // 제거됨 (3층 위 콜라이더 제거)

	//  NEW: 프레임 끝에 안전한 정리
	SafeCleanupDestroyedObjects();
}

void Aron_Scene::Exit()
{
	OutputDebugStringA("Aron_Scene Exit called\n");
	// game object -> destroy()
	__super::Exit();
}

void Aron_Scene::CheckCollisionAndChangeColor()
{
	// 알파값 기능 제거됨 - 이펙트로 대체 예정
}

void Aron_Scene::HandleHonmunMovement()
{
	float moveSpeed = 3.0f; // 키네마틱 이동 속도
	
	// Z/X 키로 오브젝트 선택 (Q는 웨이브 시작과 충돌하므로 변경)
	if (Input::GetKeyDown('Z') && !allHonmuns.empty())
	{
		selectedHonmunIndex = (selectedHonmunIndex - 1 + allHonmuns.size()) % allHonmuns.size();
		// 유효하지 않은 오브젝트는 건너뛰기
		int startIndex = selectedHonmunIndex;
		while (selectedHonmunIndex < allHonmuns.size() && 
			   (!allHonmuns[selectedHonmunIndex] || !allHonmuns[selectedHonmunIndex]->IsActive()))
		{
			selectedHonmunIndex = (selectedHonmunIndex - 1 + allHonmuns.size()) % allHonmuns.size();
			if (selectedHonmunIndex == startIndex) break; // 무한루프 방지
		}
		char debugMsg[100];
		sprintf_s(debugMsg, "Selected Honmun index: %d\n", selectedHonmunIndex);
		OutputDebugStringA(debugMsg);
	}
	if (Input::GetKeyDown('X') && !allHonmuns.empty())
	{
		selectedHonmunIndex = (selectedHonmunIndex + 1) % allHonmuns.size();
		// 유효하지 않은 오브젝트는 건너뛰기
		int startIndex = selectedHonmunIndex;
		while (!allHonmuns[selectedHonmunIndex] || !allHonmuns[selectedHonmunIndex]->IsActive())
		{
			selectedHonmunIndex = (selectedHonmunIndex + 1) % allHonmuns.size();
			if (selectedHonmunIndex == startIndex) break; // 무한루프 방지
		}
		char debugMsg[100];
		sprintf_s(debugMsg, "Selected Honmun index: %d\n", selectedHonmunIndex);
		OutputDebugStringA(debugMsg);
	}
	
	// 선택된 오브젝트 이동 (화살표 키) - 안전한 경계 체크 추가
	if (!allHonmuns.empty() && 
		selectedHonmunIndex < allHonmuns.size() && 
		selectedHonmunIndex >= 0 &&
		allHonmuns[selectedHonmunIndex] && 
		allHonmuns[selectedHonmunIndex]->IsActive())
	{
		auto* transform = allHonmuns[selectedHonmunIndex]->GetComponent<Transform>();
		if (transform)
		{
			Vector2 currentPos = transform->GetPosition();
			if (Input::GetKey(VK_LEFT)) transform->SetPosition(currentPos.x - moveSpeed, currentPos.y);
			if (Input::GetKey(VK_RIGHT)) transform->SetPosition(currentPos.x + moveSpeed, currentPos.y);
			if (Input::GetKey(VK_UP)) transform->SetPosition(currentPos.x, currentPos.y + moveSpeed);
			if (Input::GetKey(VK_DOWN)) transform->SetPosition(currentPos.x, currentPos.y - moveSpeed);
		}
	}
	else if (!allHonmuns.empty())
	{
		// 인덱스가 유효하지 않으면 초기화
		selectedHonmunIndex = 0;
	}
	
	// 웨이브 1 테스트: C, D 주석처리
	// // C 혼문 (IJKL로 상하좌우 이동)
	// if (honmun_c)
	// {
	// 	auto* rb = honmun_c->GetComponent<Rigidbody>();
	// 	if (rb)
	// 	{
	// 		if (Input::GetKey('J')) rb->AddForce(Vector2(-moveSpeed, 0));
	// 		if (Input::GetKey('L')) rb->AddForce(Vector2(moveSpeed, 0));
	// 		if (Input::GetKey('I')) rb->AddForce(Vector2(0, moveSpeed));
	// 		if (Input::GetKey('K')) rb->AddForce(Vector2(0, -moveSpeed));
	// 	}
	// }
	// 
	// // D 혼문 (8456 숫자키로 상하좌우 이동)
	// if (honmun_d)
	// {
	// 	auto* rb = honmun_d->GetComponent<Rigidbody>();
	// 	if (rb)
	// 	{
	// 		if (Input::GetKey('4')) rb->AddForce(Vector2(-moveSpeed, 0));
	// 		if (Input::GetKey('6')) rb->AddForce(Vector2(moveSpeed, 0));
	// 		if (Input::GetKey('8')) rb->AddForce(Vector2(0, moveSpeed));
	// 		if (Input::GetKey('5')) rb->AddForce(Vector2(0, -moveSpeed));
	// 	}
	// }
	
	// R키는 이제 Update()에서 ResetScene()으로 처리됨
}

void Aron_Scene::AddScore(int points)
{
	currentScore += points;
	UpdateScoreUI();
	
	// GameManager로 혼문 카운트 전송
	GameManager::Get().honCount += points;
	
	// 디버그 출력
	char debugMsg[100];
	sprintf_s(debugMsg, "Score added: +%d, Total: %d, GameManager honCount: %d\n", 
		points, currentScore, GameManager::Get().honCount);
	OutputDebugStringA(debugMsg);
}

void Aron_Scene::UpdateScoreUI()
{
	if (score_text && score_text->screenTextRenderer)
	{
		wchar_t scoreText[150];
		if (waveData.waveActive)
		{
			float elapsedTime = Time::GetTotalTime() - waveData.waveStartTime;
			float remainingTime = waveData.waveDuration - elapsedTime;
			if (remainingTime < 0) remainingTime = 0;
			
			swprintf_s(scoreText, L"Score: %d | Wave %d: %d/20 spawned | Time: %.1fs", 
				currentScore, waveData.currentWave, waveData.currentSpawnIndex, remainingTime);
		}
		else
		{
			swprintf_s(scoreText, L"Score: %d | Press Q(Wave1) E(Wave2) T(Wave3)", currentScore);
		}
		score_text->screenTextRenderer->SetText(scoreText);
	}
}

void Aron_Scene::StartWave1()
{
	OutputDebugStringA("Wave 1 started! (70 seconds)\n");
	
	// 웨이브 타이머 초기화
	waveData.waveStartTime = Time::GetTotalTime();
	waveData.waveCompleted = false;
	
	// GameManager 웨이브 정보 업데이트
	GameManager::Get().waveCount = 1;
	GameManager::Get().isWave = true;
	
	// 웨이브 시스템 완전 초기화
	waveData.waveActive = true;
	waveData.currentSpawnIndex = 0;
	waveData.lastSpawnTime = 0.0f;
	waveData.spawnInterval = 1.0f;  // 스폰 간격 초기화
	waveData.spawnedHonmuns.clear();
	
	// 기존 테스트용 혼문들 안전하게 제거
	OutputDebugStringA("Clearing existing test Honmuns safely...\n");
	
	// 안전한 복사본 생성
	std::vector<Honmun*> allHonmunsCopy;
	try {
		allHonmunsCopy = allHonmuns;
	}
	catch (...) {
		OutputDebugStringA("ERROR: Failed to copy allHonmuns vector, skipping cleanup\n");
		allHonmuns.clear();
		selectedHonmunIndex = 0;
		return;
	}
	
	OutputDebugStringA("Processing existing Honmuns for cleanup...\n");
	for (auto* honmun : allHonmunsCopy)
	{
		if (honmun) {
			try {
				if (honmun->IsActive()) {
					honmun->SetActive(false);
					OutputDebugStringA("Deactivated test Honmun\n");
				}
			}
			catch (...) {
				OutputDebugStringA("ERROR: Exception while deactivating Honmun\n");
			}
		}
	}
	
	// 벡터 안전하게 정리
	allHonmuns.clear();
	selectedHonmunIndex = 0;  // 선택 인덱스 초기화
	
	// HonmunManager도 안전하게 초기화
	if (!honmunManager.isUpdating) {
		honmunManager.activeHonmuns.clear();
		honmunManager.currentCount = 0;
		OutputDebugStringA("HonmunManager cleared for Wave 1\n");
	}
	
	OutputDebugStringA("Test Honmuns cleanup completed\n");
	
	// 웨이브 1 설정
	waveData.currentWave = 1;
	waveData.totalSpawnCount = 20;  // 웨이브 1: 20마리
	
	// 점수 초기화 (선택사항)
	currentScore = 0;
	UpdateScoreUI();
}

void Aron_Scene::StartWave2()
{
	OutputDebugStringA("Wave 2 started! (70 seconds)\n");
	
	// 웨이브 타이머 초기화
	waveData.waveStartTime = Time::GetTotalTime();
	waveData.waveCompleted = false;
	
	// GameManager 웨이브 정보 업데이트
	GameManager::Get().waveCount = 2;
	GameManager::Get().isWave = true;
	
	// 웨이브 시스템 완전 초기화
	waveData.waveActive = true;
	waveData.currentSpawnIndex = 0;
	waveData.lastSpawnTime = 0.0f;
	waveData.spawnInterval = 0.8f;  // 웨이브 2는 조금 더 빠른 스폰
	waveData.spawnedHonmuns.clear();
	
	// 기존 테스트용 혼문들 안전하게 제거 (Wave 2)
	OutputDebugStringA("Clearing existing test Honmuns for Wave 2...\n");
	
	// 안전한 복사본 생성
	std::vector<Honmun*> allHonmunsCopy;
	try {
		allHonmunsCopy = allHonmuns;
	}
	catch (...) {
		OutputDebugStringA("ERROR: Failed to copy allHonmuns vector in Wave 2, skipping cleanup\n");
		allHonmuns.clear();
		selectedHonmunIndex = 0;
		return;
	}
	
	for (auto* honmun : allHonmunsCopy)
	{
		if (honmun) {
			try {
				if (honmun->IsActive()) {
					honmun->SetActive(false);
				}
			}
			catch (...) {
				OutputDebugStringA("ERROR: Exception while deactivating Honmun in Wave 2\n");
			}
		}
	}
	allHonmuns.clear();
	selectedHonmunIndex = 0;  // 선택 인덱스 초기화
	
	// HonmunManager도 안전하게 초기화
	if (!honmunManager.isUpdating) {
		honmunManager.activeHonmuns.clear();
		honmunManager.currentCount = 0;
		OutputDebugStringA("HonmunManager cleared for Wave 2\n");
	}
	
	// 웨이브 2 설정
	waveData.currentWave = 2;
	waveData.totalSpawnCount = 25;  // 웨이브 2: 25마리 (더 많음)
	
	// 점수 초기화 (선택사항)
	currentScore = 0;
	UpdateScoreUI();
}

void Aron_Scene::StartWave3()
{
	OutputDebugStringA("Wave 3 started! (70 seconds)\n");
	
	// 웨이브 타이머 초기화
	waveData.waveStartTime = Time::GetTotalTime();
	waveData.waveCompleted = false;
	
	// GameManager 웨이브 정보 업데이트
	GameManager::Get().waveCount = 3;
	GameManager::Get().isWave = true;
	
	// 웨이브 시스템 완전 초기화
	waveData.waveActive = true;
	waveData.currentSpawnIndex = 0;
	waveData.lastSpawnTime = 0.0f;
	waveData.spawnInterval = 0.6f;  // 웨이브 3는 가장 빠른 스폰
	waveData.spawnedHonmuns.clear();
	
	// 기존 테스트용 혼문들 안전하게 제거 (Wave 3)
	OutputDebugStringA("Clearing existing test Honmuns for Wave 3...\n");
	
	// 안전한 복사본 생성
	std::vector<Honmun*> allHonmunsCopy;
	try {
		allHonmunsCopy = allHonmuns;
	}
	catch (...) {
		OutputDebugStringA("ERROR: Failed to copy allHonmuns vector in Wave 3, skipping cleanup\n");
		allHonmuns.clear();
		selectedHonmunIndex = 0;
		return;
	}
	
	for (auto* honmun : allHonmunsCopy)
	{
		if (honmun) {
			try {
				if (honmun->IsActive()) {
					honmun->SetActive(false);
				}
			}
			catch (...) {
				OutputDebugStringA("ERROR: Exception while deactivating Honmun in Wave 3\n");
			}
		}
	}
	allHonmuns.clear();
	selectedHonmunIndex = 0;  // 선택 인덱스 초기화
	
	// HonmunManager도 안전하게 초기화
	if (!honmunManager.isUpdating) {
		honmunManager.activeHonmuns.clear();
		honmunManager.currentCount = 0;
		OutputDebugStringA("HonmunManager cleared for Wave 3\n");
	}
	
	// 웨이브 3 설정
	waveData.currentWave = 3;
	waveData.totalSpawnCount = 30;  // 웨이브 3: 30마리 (가장 많음)
	
	// 점수 초기화 (선택사항)
	currentScore = 0;
	UpdateScoreUI();
}

void Aron_Scene::UpdateWaveSystem()
{
	if (!waveData.waveActive) return;
	
	//  FIXED: static 변수 제거 - 씬 전환마다 초기화되도록
	float currentTime = Time::GetTotalTime();
	
	// 웨이브 시간 체크 (70초 제한)
	float elapsedTime = currentTime - waveData.waveStartTime;
	if (elapsedTime >= waveData.waveDuration)
	{
		OutputDebugStringA("Wave time limit reached (70 seconds) - Auto advancing to next wave\n");
		CompleteCurrentWave();
		return;
	}
	
	// 스폰 간격에 따라 새로운 혼문 스폰
	if (waveData.currentSpawnIndex < waveData.totalSpawnCount)
	{
		if (currentTime - waveData.lastSpawnTime >= waveData.spawnInterval)
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
	}
	
	//  FIXED: 웨이브 완료 감지 및 자동 전환 로직 추가
	else if (waveData.currentSpawnIndex >= waveData.totalSpawnCount)
	{
		// 모든 적이 스폰되었고, 화면에 남은 적이 없으면 웨이브 완료
		if (waveData.spawnedHonmuns.size() == 0 || AllEnemiesDefeated())
		{
			CompleteCurrentWave();
		}
	}
	
	// 바닥에 도착한 혼문들 체크하여 다음 스폰 조건 확인
	for (auto* honmun : waveData.spawnedHonmuns)
	{
		if (honmun && honmun->IsActive() && IsHonmunOnGround(honmun))
		{
			// 바닥에 도착한 혼문이 있으면 스폰 간격을 줄여서 더 빨리 스폰
			waveData.spawnInterval = 0.5f; // 더 빨리 스폰
		}
	}
}

void Aron_Scene::SpawnHonmun()
{
	// TEMP FIX: 스폰 일시 중단으로 오버로드 방지
	OutputDebugStringA("SpawnHonmun: Temporarily disabled to prevent overload\n");
	return;
	
	// 랜덤 X 위치 (카메라 영역 내)
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> xDis(-1000.0f, 1000.0f);
	
	float spawnX = xDis(gen);
	
	// 새로운 혼문 생성 (Wave 1은 A, B만)
	auto* newHonmun = CreateObject<Honmun>();
	newHonmun->SetHonmunType(GetRandomHonmunTypeWave1());
	newHonmun->SetPosition(spawnX, waveData.spawnY);
	
	// 중력과 물리 활성화
	auto* rb = newHonmun->GetComponent<Rigidbody>();
	if (rb)
	{
		rb->useGravity = true;      // 중력 활성화
		rb->isKinematic = false;    // 물리 시뮬레이션 활성화
	}
	
	//  FIXED: 중복 제거 - 하나의 벡터만 사용
	waveData.spawnedHonmuns.push_back(newHonmun);
	// allHonmuns.push_back(newHonmun); // 제거: 중복 방지
	
	char debugMsg[100];
	sprintf_s(debugMsg, "Spawned Honmun %d/%d at (%.2f, %.2f)\n", 
		waveData.currentSpawnIndex + 1, waveData.totalSpawnCount, spawnX, waveData.spawnY);
	OutputDebugStringA(debugMsg);
}

bool Aron_Scene::IsHonmunOnGround(Honmun* honmun)
{
	if (!honmun || !honmun->IsActive()) return false;
	
	auto* transform = honmun->GetComponent<Transform>();
	if (!transform) return false;
	
	Vector2 pos = transform->GetPosition();
	// 2층에 도착했거나 그 아래에 있으면 "바닥 도착"으로 간주
	return pos.y <= waveData.floor2Y + 30.0f;
}

HonmunType Aron_Scene::GetRandomHonmunType()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dis(0, 1); // 0 = A, 1 = B
	
	return (dis(gen) == 0) ? HonmunType::A : HonmunType::B;
}

HonmunType Aron_Scene::GetRandomHonmunTypeWave1()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dis(0, 1); // 0 = A, 1 = B
	
	return (dis(gen) == 0) ? HonmunType::A : HonmunType::B;
}

HonmunType Aron_Scene::GetRandomHonmunTypeWave2()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dis(0, 2); // 0 = A, 1 = B, 2 = C
	
	int choice = dis(gen);
	if (choice == 0) return HonmunType::A;
	else if (choice == 1) return HonmunType::B;
	else return HonmunType::C;
}

HonmunType Aron_Scene::GetRandomHonmunTypeWave3()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dis(0, 3); // 0 = A, 1 = B, 2 = C, 3 = D
	
	int choice = dis(gen);
	if (choice == 0) return HonmunType::A;
	else if (choice == 1) return HonmunType::B;
	else if (choice == 2) return HonmunType::C;
	else return HonmunType::D;
}

void Aron_Scene::SpawnHonmunWave2()
{
	// TEMP FIX: 스폰 일시 중단으로 오버로드 방지
	OutputDebugStringA("SpawnHonmunWave2: Temporarily disabled to prevent overload\n");
	return;
	
	// 랜덤 X 위치 (카메라 영역 내)
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> xDis(-1000.0f, 1000.0f);
	
	float spawnX = xDis(gen);
	
	// 새로운 혼문 생성 (Wave 2는 A, B, C 타입 포함)
	auto* newHonmun = CreateObject<Honmun>();
	newHonmun->SetHonmunType(GetRandomHonmunTypeWave2());
	newHonmun->SetPosition(spawnX, waveData.spawnY);
	
	// 중력과 물리 활성화
	auto* rb = newHonmun->GetComponent<Rigidbody>();
	if (rb)
	{
		rb->useGravity = true;      // 중력 활성화
		rb->isKinematic = false;    // 물리 시뮬레이션 활성화
	}
	
	//  FIXED: 중복 제거 - 하나의 벡터만 사용
	waveData.spawnedHonmuns.push_back(newHonmun);
	// allHonmuns.push_back(newHonmun); // 제거: 중복 방지
	
	char debugMsg[100];
	sprintf_s(debugMsg, "Wave 2: Spawned Honmun %d/%d at (%.2f, %.2f)\n", 
		waveData.currentSpawnIndex + 1, waveData.totalSpawnCount, spawnX, waveData.spawnY);
	OutputDebugStringA(debugMsg);
}

void Aron_Scene::SpawnHonmunWave3()
{
	// TEMP FIX: 스폰 일시 중단으로 오버로드 방지
	OutputDebugStringA("SpawnHonmunWave3: Temporarily disabled to prevent overload\n");
	return;
	
	// 랜덤 X 위치 (카메라 영역 내)
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> xDis(-1000.0f, 1000.0f);
	
	float spawnX = xDis(gen);
	
	// 새로운 혼문 생성 (Wave 3는 A, B, C, D 타입 포함)
	auto* newHonmun = CreateObject<Honmun>();
	newHonmun->SetHonmunType(GetRandomHonmunTypeWave3());
	newHonmun->SetPosition(spawnX, waveData.spawnY);
	
	// 중력과 물리 활성화
	auto* rb = newHonmun->GetComponent<Rigidbody>();
	if (rb)
	{
		rb->useGravity = true;      // 중력 활성화
		rb->isKinematic = false;    // 물리 시뮬레이션 활성화
	}
	
	//  FIXED: 중복 제거 - 하나의 벡터만 사용
	waveData.spawnedHonmuns.push_back(newHonmun);
	// allHonmuns.push_back(newHonmun); // 제거: 중복 방지
	
	char debugMsg[100];
	sprintf_s(debugMsg, "Wave 3: Spawned Honmun %d/%d at (%.2f, %.2f)\n", 
		waveData.currentSpawnIndex + 1, waveData.totalSpawnCount, spawnX, waveData.spawnY);
	OutputDebugStringA(debugMsg);
}

void Aron_Scene::CheckHonmunsReachFloor1()
{
	if (!waveData.waveActive) return;
	
	// 안전한 벡터 복사를 통한 순회 (iterator 무효화 방지)
	std::vector<Honmun*> spawnedHonmunsCopy = waveData.spawnedHonmuns;
	std::vector<Honmun*> toRemove;
	
	// 1단계: 제거할 혼문들 식별
	for (auto* honmun : spawnedHonmunsCopy)
	{
		if (!honmun || !honmun->IsActive())
		{
			toRemove.push_back(honmun);
			continue;
		}
		
		auto* transform = honmun->GetComponent<Transform>();
		if (transform)
		{
			Vector2 pos = transform->GetPosition();
			// 1층 바닥(-400) 도달 체크
			if (pos.y <= -390.0f) // 바닥 근처
			{
				OutputDebugStringA("Honmun reached floor 1, destroying!\n");
				honmun->SetActive(false);
				toRemove.push_back(honmun);
			}
		}
	}
	
	// 2단계: 안전하게 제거
	for (auto* honmun : toRemove)
	{
		// 벡터에서 제거 (erase-remove idiom 사용)
		waveData.spawnedHonmuns.erase(
			std::remove(waveData.spawnedHonmuns.begin(), waveData.spawnedHonmuns.end(), honmun),
			waveData.spawnedHonmuns.end()
		);
	}
}

void Aron_Scene::HandleCameraMovement()
{
	if (!cam) return;
	
	auto* cameraTransform = cam->GetComponent<Transform>();
	if (!cameraTransform) return;
	
	Vector2 currentPos = cameraTransform->GetPosition();
	float moveSpeed = 300.0f * Time::GetDeltaTime(); // 초당 300픽셀 이동
	
	// UHJK로 카메라 이동 (플레이어 WASD와 분리)
	if (Input::GetKey('U')) // 위로
	{
		cameraTransform->SetPosition(currentPos.x, currentPos.y + moveSpeed);
	}
	if (Input::GetKey('J')) // 아래로
	{
		cameraTransform->SetPosition(currentPos.x, currentPos.y - moveSpeed);
	}
	if (Input::GetKey('H')) // 왼쪽으로
	{
		cameraTransform->SetPosition(currentPos.x - moveSpeed, currentPos.y);
	}
	if (Input::GetKey('K')) // 오른쪽으로
	{
		cameraTransform->SetPosition(currentPos.x + moveSpeed, currentPos.y);
	}
	
	// C키로 카메라 초기 위치로 리셋
	if (Input::GetKeyDown('C'))
	{
		cameraTransform->SetPosition(0.0f, 0.0f);
		OutputDebugStringA("Camera reset to origin!\n");
	}
}

void Aron_Scene::ResetScene()
{
	OutputDebugStringA("ResetScene called - complete scene reset!\n");
	
	// GameManager 상태 초기화
	GameManager::Get().waveCount = 0;
	GameManager::Get().isWave = false;
	// honCount는 유지 (누적 점수)
	
	// 웨이브 시스템 완전 정지
	waveData.waveActive = false;
	waveData.currentSpawnIndex = 0;
	waveData.lastSpawnTime = 0.0f;
	waveData.spawnInterval = 1.0f;
	waveData.spawnedHonmuns.clear();
	
	// 모든 활성 혼문들 완전 제거
	for (auto* honmun : allHonmuns)
	{
		if (honmun && honmun->IsActive())
		{
			// auto* script = honmun->GetComponent<HonmunCollisionScript>();
			// if (script)
			// {
			//	script->DestroyThis();
			// }
			honmun->SetActive(false);
		}
	}
	allHonmuns.clear();
	selectedHonmunIndex = 0;  // 선택 인덱스 초기화
	
	// 점수 초기화
	currentScore = 0;
	UpdateScoreUI();
	
	// 카메라 위치 초기화
	if (cam)
	{
		auto* cameraTransform = cam->GetComponent<Transform>();
		if (cameraTransform)
		{
			cameraTransform->SetPosition(0.0f, 0.0f);
		}
	}
	
	OutputDebugStringA("Scene reset completed!\n");
}

// ===== 혼문 관리 시스템 구현 =====

void Aron_Scene::UpdateHonmunManager()
{
	// 맵 밖으로 나간 혼문들 제거
	CheckAndRemoveOutOfBounds();
	
	// 현재 혼문 수 업데이트
	honmunManager.currentCount = static_cast<int>(honmunManager.activeHonmuns.size());
	
	// 목표 수량보다 적으면 새로 생성
	if (honmunManager.currentCount < honmunManager.targetCount)
	{
		// DeltaTime을 누적해서 시간 추적
		static float accumulatedTime = 0.0f;
		accumulatedTime += Time::GetDeltaTime();
		
		if (accumulatedTime - honmunManager.lastRespawnTime >= honmunManager.respawnInterval)
		{
			int needCount = honmunManager.targetCount - honmunManager.currentCount;
			for (int i = 0; i < needCount && i < 3; i++) // 한 번에 최대 3개씩 생성
			{
				SpawnNewHonmun();
			}
			honmunManager.lastRespawnTime = accumulatedTime;
		}
	}
	
	// 디버그 UI 업데이트 (안전한 null 체크)
	if (debug_text && debug_text->screenTextRenderer)
	{
		try {
			wchar_t debugInfo[300];
			swprintf_s(debugInfo, L"Honmuns: %d/%d | Area: 1800x720 | Camera: UHJK | Wave: Q/E | Reset: R", 
				honmunManager.currentCount, honmunManager.targetCount);
			debug_text->screenTextRenderer->SetText(debugInfo);
		} catch (...) {
			OutputDebugStringA("Exception in debug UI update\n");
		}
	}
}

void Aron_Scene::CheckAndRemoveOutOfBounds()
{
	// 동시 수정 방지: 업데이트 플래그 설정
	if (honmunManager.isUpdating) {
		OutputDebugStringA("CheckAndRemoveOutOfBounds: Already updating, skipping\n");
		return;
	}
	
	try {
		honmunManager.isUpdating = true;
	
	// 안전한 2단계 제거: 먼저 제거할 객체들을 수집, 그 다음 제거
	std::vector<Honmun*> toRemove;
	
	// CRASH FIX: vector 복사 대신 인덱스 기반 순회로 변경 (메모리 안전)
	size_t honmunCount = honmunManager.activeHonmuns.size();
	
	// CRASH FIX: 역순 인덱스 순회로 안전한 순회 보장
	for (int i = static_cast<int>(honmunCount) - 1; i >= 0; i--)
	{
		if (i < static_cast<int>(honmunManager.activeHonmuns.size())) // 크기 재확인
		{
			Honmun* honmun = honmunManager.activeHonmuns[i];
			if (!honmun || IsOutOfBounds(honmun))
			{
				toRemove.push_back(honmun);
			}
		}
	}
	
	// 2단계: 수집된 객체들을 안전하게 제거
	for (Honmun* honmun : toRemove)
	{
		if (honmun)
		{
			// CRASH FIX: 안전한 이름 접근 (GetComponent 호출 방지)
			char debugMsg[100];
			try {
				if (honmun->IsActive()) {
					sprintf_s(debugMsg, "Removing out-of-bounds Honmun: %s\n", honmun->name.c_str());
				} else {
					sprintf_s(debugMsg, "Removing inactive Honmun (already destroyed)\n");
				}
			} catch (...) {
				sprintf_s(debugMsg, "Removing corrupted Honmun (name access failed)\n");
			}
			OutputDebugStringA(debugMsg);
			
			// 안전하게 리스트에서 제거
			auto it = std::find(honmunManager.activeHonmuns.begin(), honmunManager.activeHonmuns.end(), honmun);
			if (it != honmunManager.activeHonmuns.end())
			{
				honmunManager.activeHonmuns.erase(it);
			}
			
			// CRASH FIX: 활성 상태인 경우에만 파괴 시도
			if (honmun->IsActive()) {
				honmun->Destroy();
			} else {
				OutputDebugStringA("Honmun already inactive, skipping Destroy()\n");
			}
		}
		else
		{
			// null 객체도 리스트에서 제거
			auto it = std::find(honmunManager.activeHonmuns.begin(), honmunManager.activeHonmuns.end(), honmun);
			if (it != honmunManager.activeHonmuns.end())
			{
				honmunManager.activeHonmuns.erase(it);
			}
		}
	}
	
		// 카운트 업데이트 및 플래그 해제
		honmunManager.currentCount = static_cast<int>(honmunManager.activeHonmuns.size());
		honmunManager.isUpdating = false;
	} catch (...) {
		OutputDebugStringA("Exception in CheckAndRemoveOutOfBounds, resetting flags\n");
		honmunManager.isUpdating = false;
	}
}

void Aron_Scene::SpawnNewHonmun()
{
	// 랜덤 위치 생성 (더욱 넓어진 충돌 연구 영역)
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> xDis(-1500.0f, 1500.0f); // 충돌 연구를 위해 더욱 넓게
	static std::uniform_real_distribution<float> yDis(1200.0f, 2000.0f); // 화면 훨씬 위쪽 더 높은 위치
	static std::uniform_int_distribution<int> typeDis(0, 3); // A, B, C, D 타입
	
	float spawnX = xDis(gen);
	float spawnY = yDis(gen);
	
	// 새 혼문 안전한 생성
	auto* newHonmun = CreateObject<Honmun>();
	if (!newHonmun) {
		OutputDebugStringA("CRITICAL ERROR: Failed to create new Honmun object\n");
		return;
	}
	
	// 웨이브별 타입 제한 (FIXED: 1웨이브에서 C,D 나타나는 문제 해결)
	HonmunType randomType;
	if (waveData.currentWave == 1) {
		randomType = GetRandomHonmunTypeWave1(); // A, B만
	} else if (waveData.currentWave == 2) {
		randomType = GetRandomHonmunTypeWave2(); // A, B, C
	} else if (waveData.currentWave == 3) {
		randomType = GetRandomHonmunTypeWave3(); // A, B, C, D
	} else {
		// 기본값 (웨이브 비활성 시)
		HonmunType types[] = { HonmunType::A, HonmunType::B };
		static std::uniform_int_distribution<int> defaultTypeDis(0, 1);
		randomType = types[defaultTypeDis(gen)];
	}
	
	try {
		newHonmun->SetPosition(spawnX, spawnY);
		newHonmun->SetHonmunType(randomType);
		
		// 이름 설정
		static int spawnCounter = 0;
		char honmunName[50];
		sprintf_s(honmunName, "Honmun_Auto_%d", ++spawnCounter);
		newHonmun->name = honmunName;
		
		// 물리 설정
		auto* rb = newHonmun->GetComponent<Rigidbody>();
		if (rb) {
			rb->isKinematic = false;
			rb->useGravity = false;
		}
		
		auto* collider = newHonmun->GetComponent<CircleCollider>();
		if (collider) {
			collider->isTrigger = true;
		}
		
		// 관리 리스트에 안전하게 추가
		AddHonmunToManager(newHonmun);
		
		char debugMsg[100];
		sprintf_s(debugMsg, "Spawned new Honmun: %s at (%.1f, %.1f), Type: %d\n", 
			honmunName, spawnX, spawnY, static_cast<int>(randomType));
		OutputDebugStringA(debugMsg);
		
	} catch (...) {
		OutputDebugStringA("EXCEPTION: Error setting up Honmun, destroying...\n");
		if (newHonmun) {
			newHonmun->SetActive(false);
		}
	}
}

