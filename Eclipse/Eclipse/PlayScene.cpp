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
	playUI->stopButton = CreateObject<UI_Button>();
	playUI->chatImage = CreateObject<UI_Image>();

	npc = CreateObject<NPC>();

	player = CreateObject<Player_Test>();

	// title sample
	title_text = CreateObject<UI_Text>();
	title_text->rectTransform->SetPosition(0, 500);
	title_text->rectTransform->SetSize(500, 100);
	title_text->screenTextRenderer->SetFontSize(50);
	title_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::FloralWhite));
	title_text->screenTextRenderer->SetText(L"Play Scene");
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

void PlayScene::ChatOpen()
{

}
