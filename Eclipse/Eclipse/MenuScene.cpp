#include "MenuScene.h"
#include "EclipseApp.h"
#include "../Direct2D_EngineLib/SceneManager.h"

void MenuScene::Awake()
{
	// camera init
	cam = CreateObject<GameObject>();
	cam->AddComponent<Transform>();
	cam->AddComponent<Camera>(1920, 1080);

	// create gameobject
	titleUI = CreateObject<TitleUI>();
	titleUI->backgroundImage = CreateObject<GameObject>();
	titleUI->titleLogo_Image = CreateObject<GameObject>();
	titleUI->titleSpell_Image = CreateObject<GameObject>();
	titleUI->play_Button = CreateObject<UI_Button>();
	titleUI->options_Button = CreateObject<UI_Button>();
	titleUI->credit_Button = CreateObject<UI_Button>();
	titleUI->end_Button = CreateObject<UI_Button>();
	titleUI->underscore_Image = CreateObject<UI_Image>();

	titleUI->optionUI = CreateObject<Options>();
	titleUI->creditUI = CreateObject<Credit>();
}

void MenuScene::Start()
{
	// game object -> SceneStart()
	__super::Start();
}

void MenuScene::Update()
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

void MenuScene::Exit()
{
	// game object -> destroy()
	__super::Exit();
}