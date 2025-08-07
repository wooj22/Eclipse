#include "PlayScene.h"
#include "EclipseApp.h"
#include "../Direct2D_EngineLib/SceneManager.h"

void PlayScene::Awake()
{
	// camera init
	cam = CreateObject<GameObject>();
	cam->AddComponent<Transform>();
	cam->AddComponent<Camera>(1920, 1080);

	// create gameobject
	backGround = CreateObject<GameObject>();
	backGround->AddComponent<Transform>();
	backGround->AddComponent<SpriteRenderer>();
	auto playbg = ResourceManager::Get().CreateTexture2D("../Resource/mo/PlayBackGround.png");
	backGround->GetComponent<SpriteRenderer>()->sprite = ResourceManager::Get().CreateSprite(playbg, "PlayBackGround");
	backGround->GetComponent<Transform>()->SetScale(3, 3);

	//wave
	waveSystemObj = CreateObject<GameObject>();
	waveSystemObj->name = "WaveSystem";
	waveSystemObj->SetActive(true);  // GameObject 활성화
	waveSystem = waveSystemObj->AddComponent<WaveSystem>();

	player = CreateObject<Player_Test>();

	playUI = CreateObject<PlayUI>();
	GameManager::Get().g_playUI = playUI;	
}

void PlayScene::Start()
{
	// game object -> SceneStart()
	__super::Start();

}

void PlayScene::Update()
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

	if (Input::GetKeyDown(VK_F3))
	{
		GameManager::Get().ChangeHonCount(149); // ȥ ġƮ
	}

	if (Input::GetKeyDown(VK_F4))
	{
		GameManager::Get().ChangeHonCount(1); // ȥ ġƮ
	}

	if (Input::GetKeyDown(VK_F5))
	{
		GameManager::Get().AllSkillUnlock(); // ��� ��ų �ر� ġƮ
	}

	if (Input::GetKeyDown(VK_F6))
	{
		GameManager::Get().SkillReset();
	}

	if (Input::GetKeyDown(VK_F7))
	{
		GameManager::Get().CanAbsorb();
	}

	if (Input::GetKeyDown(VK_F8))
	{
		GameManager::Get().UseAbsorb();
		GameManager::Get().absorbCoolTime = 5;
	}

	if (Input::GetKeyDown(VK_F9))
	{
		GameManager::Get().CanRelease();
	}

	if (Input::GetKeyDown(VK_F10))
	{
		GameManager::Get().UseRelease();
	}

	if (Input::GetKeyDown(VK_F11))
	{
		GameManager::Get().isWave = false;
		GameManager::Get().FinishWaveTimeText();
	}
}

void PlayScene::Exit()
{
	// game object -> destroy()
	GameManager::Get().ReSetData();
	__super::Exit();
}

void PlayScene::ChagneEndScene()
{
	SceneManager::Get().ChangeScene(EclipseApp::END);
}