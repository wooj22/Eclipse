#include "EndScene.h"
#include "EclipseApp.h"
#include "../Direct2D_EngineLib/SceneManager.h"

void EndScene::Awake()
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
	title_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::FloralWhite));
	title_text->screenTextRenderer->SetText(L"End Scene");
}

void EndScene::Start()
{
	// game object -> SceneStart()
	__super::Start();

}

void EndScene::Update()
{
	// game object -> Update()
	__super::Update();

}

void EndScene::Exit()
{
	// game object -> destroy()
	__super::Exit();
}
