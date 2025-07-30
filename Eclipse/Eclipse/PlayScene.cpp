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

	playUI = CreateObject<PlayUI>();
	playUI->timer_Text = CreateObject<UI_Text>();
	playUI->stop_Button = CreateObject<UI_Button>();
	playUI->quest_Image = CreateObject<UI_Image>();
	playUI->quest_Text = CreateObject<UI_Text>();
	playUI->chat_Image = CreateObject<UI_Image>();
	playUI->chat_Text = CreateObject<UI_Text>();
	playUI->chat_Button= CreateObject<UI_Button>();
	playUI->hon_Image = CreateObject<UI_Image>();
	playUI->hon_Text = CreateObject<UI_Text>();
	playUI->skill1_Image = CreateObject<UI_Image>();
	playUI->skill1_Text = CreateObject<UI_Text>();
	playUI->skill2_Image = CreateObject<UI_Image>();
	playUI->skill2_Text = CreateObject<UI_Text>();

	npc = CreateObject<NPC>();

	player = CreateObject<Player_Test>();
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
}

void PlayScene::Exit()
{
	// game object -> destroy()
	__super::Exit();
}

void PlayScene::ChagneEndScene()
{
	SceneManager::Get().ChangeScene(EclipseApp::END);
}