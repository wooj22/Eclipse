#include "Aron_Scene.h"
#include "EclipseApp.h"
#include "Honmun.h"  // Honmun 클래스 포함
#include "HonmunCollisionScript.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Time.h"

void Aron_Scene::Awake()
{
	// camera init
	cam = CreateObject<GameObject>();
	cam->AddComponent<Transform>();
	cam->AddComponent<Camera>(1920, 1080);

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
<<<<<<< HEAD
	debug_text->screenTextRenderer->SetText(L"SPACE: Start Wave 1 | WASD: Camera | C: Reset Cam | Q/E: Select | R: Reset Scene | 3: Restart");
=======
	debug_text->screenTextRenderer->SetText(L"Q/E: Select, Arrow: Move, R: Reset");
>>>>>>> d9bcc8974b8f00e26a9dcd5e38c0b519500617db

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

	// 모든 혼문을 벡터에 추가
	allHonmuns.push_back(honmun_a);
	allHonmuns.push_back(honmun_a2);
	allHonmuns.push_back(honmun_a3);
	allHonmuns.push_back(honmun_a4);
	allHonmuns.push_back(honmun_b);
	allHonmuns.push_back(honmun_b2);
	allHonmuns.push_back(honmun_b3);
	allHonmuns.push_back(honmun_b4);
	
	// 웨이브 시스템 초기화
	waveData.waveActive = false;
	waveData.currentSpawnIndex = 0;
	waveData.lastSpawnTime = 0.0f;
	waveData.spawnInterval = 1.0f;
	waveData.spawnedHonmuns.clear();
	currentScore = 0;

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
	
	// 2층 플랫폼 (혼문이 통과 가능)
	floor2 = CreateObject<GameObject>();
	floor2->name = "Ground_Floor2";
	floor2->AddComponent<Transform>()->SetPosition(0.0f, -200.0f); // 중간 높이
	auto floor2_sr = floor2->AddComponent<SpriteRenderer>();
	floor2_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Ground.png"), "Ground");
	floor2_sr->layer = 0;
	floor2_col = floor2->AddComponent<BoxCollider>();
	floor2_col->size = { 1110.0f, 20.0f }; // 조금 더 얇게
	floor2_col->isTrigger = true; // 트리거로 설정 (통과 가능)
	auto floor2_rb = floor2->AddComponent<Rigidbody>();
	floor2_rb->useGravity = false;
	floor2_rb->isKinematic = true;
	// 2층 플랫폼은 트리거로 설정됨 (통과 가능)
}

void Aron_Scene::Start()
{
	// game object -> SceneStart()
	__super::Start();
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
	
	// 웨이브 1 시작 (스페이스바) - 활성화되지 않은 경우에만
	if (Input::GetKeyDown(VK_SPACE))
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
	
	// R키로 씬 완전 초기화
	if (Input::GetKeyDown('R'))
	{
		ResetScene();
	}
	
	// 카메라 이동 (WASD 키)
	HandleCameraMovement();

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

	// AABB 그리기 - 바닥들 (모든 층 표시)
	if (ground_col) ground_col->DebugColliderDraw(); // 1층 (파괴)
	if (floor2_col) floor2_col->DebugColliderDraw(); // 2층 (통과)

	
}

void Aron_Scene::Exit()
{
	OutputDebugStringA("�� ��������� ��");
	// game object -> destroy()
	__super::Exit();
}

void Aron_Scene::CheckCollisionAndChangeColor()
{
	// �����̽��ٸ� ������ ��� ȥ���� �������ϰ� �����
	if (Input::GetKeyDown(VK_SPACE))
	{
		if (honmun_a) honmun_a->SetAlpha(0.5f);
		if (honmun_b) honmun_b->SetAlpha(0.5f);
		// 웨이브 1 테스트: C, D 주석처리
		// if (honmun_c) honmun_c->SetAlpha(0.5f);
		// if (honmun_d) honmun_d->SetAlpha(0.5f);
	}

	// �����̽��ٸ� ���� ���� �������� ����
	if (Input::GetKeyUp(VK_SPACE))
	{
		if (honmun_a) honmun_a->ResetAlpha();
		if (honmun_b) honmun_b->ResetAlpha();
		// 웨이브 1 테스트: C, D 주석처리
		// if (honmun_c) honmun_c->ResetAlpha();
		// if (honmun_d) honmun_d->ResetAlpha();
	}
}

void Aron_Scene::HandleHonmunMovement()
{
	float moveSpeed = 3.0f; // 키네마틱 이동 속도
	
	// Q/E 키로 오브젝트 선택
	if (Input::GetKeyDown('Q'))
	{
		selectedHonmunIndex = (selectedHonmunIndex - 1 + allHonmuns.size()) % allHonmuns.size();
		// 유효하지 않은 오브젝트는 건너뛰기
		while (selectedHonmunIndex < allHonmuns.size() && 
			   (!allHonmuns[selectedHonmunIndex] || !allHonmuns[selectedHonmunIndex]->IsActive()))
		{
			selectedHonmunIndex = (selectedHonmunIndex - 1 + allHonmuns.size()) % allHonmuns.size();
		}
		char debugMsg[100];
		sprintf_s(debugMsg, "Selected Honmun index: %d\n", selectedHonmunIndex);
		OutputDebugStringA(debugMsg);
	}
	if (Input::GetKeyDown('E'))
	{
		selectedHonmunIndex = (selectedHonmunIndex + 1) % allHonmuns.size();
		// 유효하지 않은 오브젝트는 건너뛰기
		while (selectedHonmunIndex < allHonmuns.size() && 
			   (!allHonmuns[selectedHonmunIndex] || !allHonmuns[selectedHonmunIndex]->IsActive()))
		{
			selectedHonmunIndex = (selectedHonmunIndex + 1) % allHonmuns.size();
		}
		char debugMsg[100];
		sprintf_s(debugMsg, "Selected Honmun index: %d\n", selectedHonmunIndex);
		OutputDebugStringA(debugMsg);
	}
	
	// 선택된 오브젝트 이동 (화살표 키)
	if (selectedHonmunIndex < allHonmuns.size() && 
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
	
	// 디버그 출력
	char debugMsg[100];
	sprintf_s(debugMsg, "Score added: +%d, Total: %d\n", points, currentScore);
	OutputDebugStringA(debugMsg);
}

void Aron_Scene::UpdateScoreUI()
{
	if (score_text && score_text->screenTextRenderer)
	{
		wchar_t scoreText[100];
		if (waveData.waveActive)
		{
			swprintf_s(scoreText, L"Score: %d | Wave 1: %d/%d spawned", 
				currentScore, waveData.currentSpawnIndex, waveData.totalSpawnCount);
		}
		else
		{
			swprintf_s(scoreText, L"Score: %d | Press SPACE for Wave 1", currentScore);
		}
		score_text->screenTextRenderer->SetText(scoreText);
	}
}

<<<<<<< HEAD
void Aron_Scene::StartWave1()
{
	OutputDebugStringA("Wave 1 started!\n");
	
	// 웨이브 시스템 완전 초기화
	waveData.waveActive = true;
	waveData.currentSpawnIndex = 0;
	waveData.lastSpawnTime = 0.0f;
	waveData.spawnInterval = 1.0f;  // 스폰 간격 초기화
	waveData.spawnedHonmuns.clear();
	
	// 기존 테스트용 혼문들 완전 제거
	for (auto* honmun : allHonmuns)
	{
		if (honmun && honmun->IsActive())
		{
			// 스크립트의 DestroyThis 호출로 완전 파괴
			auto* script = honmun->GetComponent<HonmunCollisionScript>();
			if (script)
			{
				script->DestroyThis();
			}
			else
			{
				honmun->SetActive(false);
			}
		}
	}
	allHonmuns.clear();
	
	// 점수 초기화 (선택사항)
	currentScore = 0;
	UpdateScoreUI();
}

void Aron_Scene::UpdateWaveSystem()
{
	if (!waveData.waveActive) return;
	
	static float totalTime = 0.0f;
	totalTime += Time::GetDeltaTime();
	float currentTime = totalTime;
	
	// 스폰 간격에 따라 새로운 혼문 스폰
	if (waveData.currentSpawnIndex < waveData.totalSpawnCount)
	{
		if (currentTime - waveData.lastSpawnTime >= waveData.spawnInterval)
		{
			SpawnHonmun();
			waveData.lastSpawnTime = currentTime;
			waveData.currentSpawnIndex++;
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
	// 랜덤 X 위치 (카메라 영역 내)
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> xDis(-400.0f, 400.0f);
	
	float spawnX = xDis(gen);
	
	// 새로운 혼문 생성
	auto* newHonmun = CreateObject<Honmun>();
	newHonmun->SetHonmunType(GetRandomHonmunType());
	newHonmun->SetPosition(spawnX, waveData.spawnY);
	
	// 중력과 물리 활성화
	auto* rb = newHonmun->GetComponent<Rigidbody>();
	if (rb)
	{
		rb->useGravity = true;      // 중력 활성화
		rb->isKinematic = false;    // 물리 시뮬레이션 활성화
	}
	
	// 리스트에 추가
	waveData.spawnedHonmuns.push_back(newHonmun);
	allHonmuns.push_back(newHonmun);
	
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

void Aron_Scene::CheckHonmunsReachFloor1()
{
	if (!waveData.waveActive) return;
	
	// 스폰된 혼문들을 체크
	for (auto it = waveData.spawnedHonmuns.begin(); it != waveData.spawnedHonmuns.end();)
	{
		auto* honmun = *it;
		if (!honmun || !honmun->IsActive())
		{
			it = waveData.spawnedHonmuns.erase(it);
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
				
				// 혼문 파괴
				auto* honmunScript = honmun->GetComponent<HonmunCollisionScript>();
				if (honmunScript)
				{
					honmunScript->DestroyThis();
				}
				
				it = waveData.spawnedHonmuns.erase(it);
				continue;
			}
		}
		++it;
	}
}

void Aron_Scene::HandleCameraMovement()
{
	if (!cam) return;
	
	auto* cameraTransform = cam->GetComponent<Transform>();
	if (!cameraTransform) return;
	
	Vector2 currentPos = cameraTransform->GetPosition();
	float moveSpeed = 300.0f * Time::GetDeltaTime(); // 초당 300픽셀 이동
	
	// WASD로 카메라 이동
	if (Input::GetKey('W')) // 위로
	{
		cameraTransform->SetPosition(currentPos.x, currentPos.y + moveSpeed);
	}
	if (Input::GetKey('S')) // 아래로
	{
		cameraTransform->SetPosition(currentPos.x, currentPos.y - moveSpeed);
	}
	if (Input::GetKey('A')) // 왼쪽으로
	{
		cameraTransform->SetPosition(currentPos.x - moveSpeed, currentPos.y);
	}
	if (Input::GetKey('D')) // 오른쪽으로
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
			auto* script = honmun->GetComponent<HonmunCollisionScript>();
			if (script)
			{
				script->DestroyThis();
			}
		}
	}
	allHonmuns.clear();
	
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
=======
void Aron_Scene::AddScore(int points)
{
	currentScore += points;
	UpdateScoreUI();
	
	// 디버그 출력
	char debugMsg[100];
	sprintf_s(debugMsg, "Score added: +%d, Total: %d\n", points, currentScore);
	OutputDebugStringA(debugMsg);
}

void Aron_Scene::UpdateScoreUI()
{
	if (score_text && score_text->screenTextRenderer)
	{
		wchar_t scoreText[50];
		swprintf_s(scoreText, L"Score: %d", currentScore);
		score_text->screenTextRenderer->SetText(scoreText);
	}
>>>>>>> d9bcc8974b8f00e26a9dcd5e38c0b519500617db
}