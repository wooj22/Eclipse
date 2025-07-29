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
	titleUI->backgroundImage = CreateObject<UI_Image>();
	titleUI->playButton = CreateObject<UI_Button>();
	titleUI->optionsButton = CreateObject<UI_Button>();
	titleUI->creditButton = CreateObject<UI_Button>();
	titleUI->endButton = CreateObject<UI_Button>();
	// title sample
	title_text = CreateObject<UI_Text>();
	title_text->rectTransform->SetPosition(0, 500);
	title_text->rectTransform->SetSize(500, 100);
	title_text->screenTextRenderer->SetFontSize(50);
	title_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::FloralWhite));
	title_text->screenTextRenderer->SetText(L"Menu Scene");

}

void MenuScene::Start()
{
	// game object -> SceneStart()
	__super::Start();

	titleUI->playButton->button->onClickListeners.AddListener(
		this, std::bind(&MenuScene::ChagnePlayScene, this));
}

void MenuScene::Update()
{
	// game object -> Update()
	__super::Update();

}

void MenuScene::Exit()
{
	// game object -> destroy()
	__super::Exit();
}

void MenuScene::ChagnePlayScene()
{
	SceneManager::Get().ChangeScene(EclipseApp::PLAY);
}