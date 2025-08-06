#include "Aron_Scene.h"
#include "EclipseApp.h"
#include "WaveSystem.h"
#include "HonA.h"
#include "HonB.h"
#include "HonC.h"
#include "HonD.h"
#include "Player.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

void Aron_Scene::Awake()
{
	// Camera init
	cam = CreateObject<GameObject>();
	cam->AddComponent<Transform>();
	cam->AddComponent<Camera>(1920, 1080);

	// Create UI
	// Title
	title_text = CreateObject<UI_Text>();
	title_text->rectTransform->SetPosition(0, 500);
	title_text->rectTransform->SetSize(500, 100);
	title_text->screenTextRenderer->SetFontSize(50);
	title_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::LightBlue));
	title_text->screenTextRenderer->SetText(L"Aron Scene - Wave System Test");
	
	// Score UI
	score_text = CreateObject<UI_Text>();
	score_text->rectTransform->SetPosition(0, 400);
	score_text->rectTransform->SetSize(600, 50);
	score_text->screenTextRenderer->SetFontSize(30);
	score_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));
	score_text->screenTextRenderer->SetText(L"Score: 0");
	
	// Debug info UI
	debug_text = CreateObject<UI_Text>();
	debug_text->rectTransform->SetPosition(0, 300);
	debug_text->rectTransform->SetSize(800, 50);
	debug_text->screenTextRenderer->SetFontSize(20);
	debug_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	debug_text->screenTextRenderer->SetText(L"F5-F8: Start Waves | 1-7: Scene Change | WASD: Camera | C: Reset Cam | R: Reset Scene");

	// Create Player (required for Hon controllers)
	auto player = CreateObject<Player>();
	player->name = "Player";
	// Position player at center bottom
	player->transform->SetPosition(0.0f, -300.0f);

	// Create Wave System
	waveSystemObj = CreateObject<GameObject>();
	waveSystemObj->name = "WaveSystem";
	waveSystem = waveSystemObj->AddComponent<WaveSystem>();

	// Ground system - Floor 1 (destruction) and Floor 2 (passthrough)
	// Floor 1 (Hons are destroyed when they reach this)
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
	
	// Floor 2 (Hons can pass through)
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

	// Initialize score
	currentScore = 0;
}

void Aron_Scene::Start()
{
	// Call base class Start
	__super::Start();
}

void Aron_Scene::Update()
{
	// Call base class Update
	__super::Update();

	// Update UI
	UpdateScoreUI();
	
	// Wave system controls (F5-F8)
	if (Input::GetKeyDown(VK_F5))
	{
		if (waveSystem && !waveSystem->IsWaveActive())
		{
			waveSystem->StartWave(1);
			OutputDebugStringA("Starting Wave 1!\n");
		}
	}
	if (Input::GetKeyDown(VK_F6))
	{
		if (waveSystem && !waveSystem->IsWaveActive())
		{
			waveSystem->StartWave(2);
			OutputDebugStringA("Starting Wave 2!\n");
		}
	}
	if (Input::GetKeyDown(VK_F7))
	{
		if (waveSystem && !waveSystem->IsWaveActive())
		{
			waveSystem->StartWave(3);
			OutputDebugStringA("Starting Wave 3!\n");
		}
	}
	if (Input::GetKeyDown(VK_F8))
	{
		if (waveSystem && !waveSystem->IsWaveActive())
		{
			waveSystem->StartWave(4);
			OutputDebugStringA("Starting Boss Wave!\n");
		}
	}
	
	// Reset scene
	if (Input::GetKeyDown('R'))
	{
		ResetScene();
	}
	
	// Camera movement
	HandleCameraMovement();

	// Scene change shortcuts (1-7)
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
		// '3' 키로 Aron 씬 재시작
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

	// Draw colliders for debugging
	if (ground_col) ground_col->DebugColliderDraw();
	if (floor2_col) floor2_col->DebugColliderDraw();
}

void Aron_Scene::Exit()
{
	OutputDebugStringA("Aron Scene Exit\n");
	// Call base class Exit
	__super::Exit();
}

void Aron_Scene::HandleCameraMovement()
{
	if (!cam) return;
	
	auto* cameraTransform = cam->GetComponent<Transform>();
	if (!cameraTransform) return;
	
	Vector2 currentPos = cameraTransform->GetPosition();
	float moveSpeed = 300.0f * Time::GetDeltaTime();
	
	// WASD camera movement
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
	
	// C key to reset camera
	if (Input::GetKeyDown('C'))
	{
		cameraTransform->SetPosition(0.0f, 0.0f);
		OutputDebugStringA("Camera reset to origin!\n");
	}
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
	if (score_text && score_text->screenTextRenderer)
	{
		wchar_t scoreText[256];
		
		if (waveSystem && waveSystem->IsWaveActive())
		{
			swprintf_s(scoreText, L"Score: %d | Wave State: %d | Spawned: %d | Destroyed: %d", 
				currentScore, 
				static_cast<int>(waveSystem->GetCurrentWaveState()),
				waveSystem->GetSpawnedCount(),
				waveSystem->GetDestroyedCount());
		}
		else
		{
			swprintf_s(scoreText, L"Score: %d | Press F5-F8 to start waves", currentScore);
		}
		
		score_text->screenTextRenderer->SetText(scoreText);
	}
}

void Aron_Scene::ResetScene()
{
	OutputDebugStringA("ResetScene called - complete scene reset!\n");
	
	// Reset wave system
	if (waveSystem)
	{
		waveSystem->ResetWaveSystem();
	}
	
	// Reset score
	currentScore = 0;
	UpdateScoreUI();
	
	// Reset camera
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