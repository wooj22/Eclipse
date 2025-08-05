#include "Aron_Scene.h"
#include "EclipseApp.h"
#include "WaveSystem.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

void Aron_Scene::Awake()
{
	// Camera 초기화
	cam = CreateObject<GameObject>();
	cam->AddComponent<Transform>();
	cam->AddComponent<Camera>(1920, 1080);

	// UI 초기화
	// 제목 텍스트
	title_text = CreateObject<UI_Text>();
	title_text->rectTransform->SetPosition(0, 500);
	title_text->rectTransform->SetSize(500, 100);
	title_text->screenTextRenderer->SetFontSize(50);
	title_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::LightBlue));
	title_text->screenTextRenderer->SetText(L"Aron Scene - Wave System");

	// 웨이브 상태 UI
	wave_status_text = CreateObject<UI_Text>();
	wave_status_text->rectTransform->SetPosition(0, 200);
	wave_status_text->rectTransform->SetSize(600, 40);
	wave_status_text->screenTextRenderer->SetFontSize(24);
	wave_status_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Cyan));
	wave_status_text->screenTextRenderer->SetText(L"Wave Ready");

	// 타이머 UI
	timer_text = CreateObject<UI_Text>();
	timer_text->rectTransform->SetPosition(400, 450);
	timer_text->rectTransform->SetSize(200, 40);
	timer_text->screenTextRenderer->SetFontSize(28);
	timer_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Orange));
	timer_text->screenTextRenderer->SetText(L"--:--");

	// 진행 상황 UI
	progress_text = CreateObject<UI_Text>();
	progress_text->rectTransform->SetPosition(0, 100);
	progress_text->rectTransform->SetSize(500, 30);
	progress_text->screenTextRenderer->SetFontSize(20);
	progress_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::LightGreen));
	progress_text->screenTextRenderer->SetText(L"Active: 0 | Spawned: 0 | Defeated: 0");

	// 디버그 정보 UI
	debug_text = CreateObject<UI_Text>();
	debug_text->rectTransform->SetPosition(0, 300);
	debug_text->rectTransform->SetSize(800, 50);
	debug_text->screenTextRenderer->SetFontSize(18);
	debug_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	debug_text->screenTextRenderer->SetText(L"H/J/K/L: Wave1~4 | WASD: Camera | C: Reset Cam | R: Reset Scene");

	// Ground 시스템 초기화
	// 1층 바닥 (파괴 감지용)
	ground = CreateObject<GameObject>();
	ground->name = "Ground_Floor1";
	ground->AddComponent<Transform>()->SetPosition(0.0f, -400.0f);
	auto ground_sr = ground->AddComponent<SpriteRenderer>();
	ground_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Ground.png"), "Ground");
	ground_sr->layer = 0;
	ground_col = ground->AddComponent<BoxCollider>();
	ground_col->size = { 1110.0f, 30.0f };
	ground_col->isTrigger = true;
	auto ground_rb = ground->AddComponent<Rigidbody>();
	ground_rb->useGravity = false;
	ground_rb->isKinematic = true;

	// 2층 플랫폼 (통과 가능)
	floor2 = CreateObject<GameObject>();
	floor2->name = "Ground_Floor2";
	floor2->AddComponent<Transform>()->SetPosition(0.0f, -200.0f);
	auto floor2_sr = floor2->AddComponent<SpriteRenderer>();
	floor2_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Ground.png"), "Ground");
	floor2_sr->layer = 0;
	floor2_col = floor2->AddComponent<BoxCollider>();
	floor2_col->size = { 1110.0f, 20.0f };
	floor2_col->isTrigger = true;
	auto floor2_rb = floor2->AddComponent<Rigidbody>();
	floor2_rb->useGravity = false;
	floor2_rb->isKinematic = true;

	// 웨이브 시스템 초기화
	InitializeWaveSystem();
	
	// 점수 초기화
	currentScore = 0;
	
	// 폰트 설정
	SetupFonts();
}

void Aron_Scene::Start()
{
	__super::Start();
}

void Aron_Scene::Update()
{
	__super::Update();

	// 웨이브 시스템 업데이트
	if (waveSystem)
	{
		waveSystem->UpdateWave(Time::GetDeltaTime());
	}

	// UI 업데이트
	UpdateScoreUI();
	UpdateWaveUI();

	// 웨이브 시작 키 입력
	if (Input::GetKeyDown('H'))
	{
		StartWave(0);  // 웨이브 1
	}
	if (Input::GetKeyDown('J'))
	{
		StartWave(1);  // 웨이브 2
	}
	if (Input::GetKeyDown('K'))
	{
		StartWave(2);  // 웨이브 3
	}
	if (Input::GetKeyDown('L'))
	{
		StartWave(3);  // 보스 웨이브
	}

	// R키로 씬 완전 초기화
	if (Input::GetKeyDown('R'))
	{
		ResetScene();
	}

	// 카메라 이동 (WASD 키)
	HandleCameraMovement();

	// 씬 전환
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

	// 디버그 그리기
	if (ground_col) ground_col->DebugColliderDraw();
	if (floor2_col) floor2_col->DebugColliderDraw();
}

void Aron_Scene::Exit()
{
	OutputDebugStringA("Aron Scene Exit\n");
	
	// 웨이브 시스템 정리
	if (waveSystem)
	{
		delete waveSystem;
		waveSystem = nullptr;
	}
	
	__super::Exit();
}

void Aron_Scene::HandleCameraMovement()
{
	if (!cam) return;
	
	auto* cameraTransform = cam->GetComponent<Transform>();
	if (!cameraTransform) return;
	
	Vector2 currentPos = cameraTransform->GetPosition();
	float moveSpeed = 300.0f * Time::GetDeltaTime();
	
	// WASD로 카메라 이동
	if (Input::GetKey('W'))
	{
		cameraTransform->SetPosition(currentPos.x, currentPos.y + moveSpeed);
	}
	if (Input::GetKey('S'))
	{
		cameraTransform->SetPosition(currentPos.x, currentPos.y - moveSpeed);
	}
	if (Input::GetKey('A'))
	{
		cameraTransform->SetPosition(currentPos.x - moveSpeed, currentPos.y);
	}
	if (Input::GetKey('D'))
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

void Aron_Scene::InitializeWaveSystem()
{
	if (waveSystem)
	{
		delete waveSystem;
	}
	waveSystem = new WaveSystem(this);
	OutputDebugStringA("Wave system initialized!\n");
}

void Aron_Scene::UpdateWaveUI()
{
	if (!waveSystem) return;
	
	// 웨이브 상태 텍스트 업데이트
	if (wave_status_text && wave_status_text->screenTextRenderer)
	{
		std::wstring statusText = waveSystem->GetWaveStatusText();
		wave_status_text->screenTextRenderer->SetText(statusText.c_str());
	}
	
	// 타이머 텍스트 업데이트
	if (timer_text && timer_text->screenTextRenderer)
	{
		std::wstring timerText = waveSystem->GetTimerText();
		timer_text->screenTextRenderer->SetText(timerText.c_str());
	}
	
	// 진행 상황 텍스트 업데이트
	if (progress_text && progress_text->screenTextRenderer)
	{
		std::wstring progressText = waveSystem->GetProgressText();
		progress_text->screenTextRenderer->SetText(progressText.c_str());
	}
}

void Aron_Scene::StartWave(int waveIndex)
{
	if (!waveSystem) return;
	
	// 웨이브 진행 중이면 강제로 다음 웨이브로 전환
	if (waveSystem->IsWaveActive())
	{
		waveSystem->ResetSystem();
		OutputDebugStringA("Previous wave stopped, starting new wave.\n");
	}
	
	waveSystem->StartWave(waveIndex);
	
	char debugMsg[100];
	sprintf_s(debugMsg, "Starting wave %d from Aron_Scene\n", waveIndex + 1);
	OutputDebugStringA(debugMsg);
}

void Aron_Scene::ResetScene()
{
	OutputDebugStringA("ResetScene called - complete scene reset!\n");
	
	// 웨이브 시스템 리셋
	if (waveSystem)
	{
		waveSystem->ResetSystem();
	}
	
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

void Aron_Scene::SetupFonts()
{
	// Noto Serif KR 폰트 사용 (아론 파트 전용)
	if (title_text && title_text->screenTextRenderer)
	{
		title_text->screenTextRenderer->SetFontName(L"Noto Serif KR");
	}
	
	if (wave_status_text && wave_status_text->screenTextRenderer)
	{
		wave_status_text->screenTextRenderer->SetFontName(L"Noto Serif KR");
	}
	
	OutputDebugStringA("Noto Serif KR font applied to Aron scene UI elements\n");
}

void Aron_Scene::AddScore(int points)
{
	currentScore += points;
	UpdateScoreUI();
	
	char debugMsg[100];
	sprintf_s(debugMsg, "Score added: +%d, Total: %d\n", points, currentScore);
	OutputDebugStringA(debugMsg);
}

void Aron_Scene::UpdateScoreUI()
{
	// 점수 UI는 현재 구현하지 않음 (필요시 추가)
}