#include "Woo_Scene.h"
#include "EclipseApp.h"
#include "../Direct2D_EngineLib/SceneManager.h"

void Woo_Scene::Awake()
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
	title_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::MediumPurple));
	title_text->screenTextRenderer->SetText(L"Engine Test Scene");
	//title_text->screenTextRenderer->SetAlpha(0.5);

	// background sample
	map = CreateObject<GameObject>();
	map->AddComponent<Transform>();
	auto sr = map->AddComponent<SpriteRenderer>();
	auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Sample/MapBackground.jpg");
	auto new_sprite = ResourceManager::Get().CreateSprite(texture, "MapBackground");
	sr->sprite = new_sprite;
	sr->layer = -1;
	map->transform->Scaleing(2, 2);
	map->transform->SetPosition(0, -500);

	// player sample
	player = CreateObject<Player_Woo>({ 0, -500 });
	cam->transform->SetPosition(player->transform->GetPosition());

	// boss
	boss = CreateObject<Boss>();

	// map sample
	map_woo = CreateObject<Map_Woo>({ 0, -900 });
	ground1 = CreateObject<Map_Woo>({ 100, -700 });
	ground2 = CreateObject<Map_Woo>({ -300, -700 });
	ground3 = CreateObject<Map_Woo>({ 300, -500 });
	ground1->collider->size = {100,20};
	ground2->collider->size = { 100,20 };
	ground3->collider->size = { 100,20 };

	// ui test
	/*parent = CreateObject<UI_Image>();
	child = CreateObject<UI_Image>();
	parent->rectTransform->SetPosition(100, 0);
	child->rectTransform->SetParent(parent->rectTransform);
	child->rectTransform->SetPosition(100, 0);

	child->imageRenderer->SetAlpha(0.1);*/

	// camera tartget
	Camera* camera = cam->GetComponent<Camera>();
	camera->SetTarget(player->transform);
	camera->SetTargetTraceSpeed(250.0f);
	camera->SetTargetTraceLimitX(30.0f);
	camera->SetTargetTraceLimitY(50.0f);

	// camera map condition
	Rect mapRect;
	mapRect.size = { 2560, 1920 }; // map position
	camera->SetMapCondition(mapRect);
}

void Woo_Scene::Start()
{
	// game object -> SceneStart()
	__super::Start();

}

void Woo_Scene::Update()
{
	// game object -> Update()
	__super::Update();

	// camera test
	// 이런 느낌에다가 offset, offset limit 주기
	//Vector2 dir = (player->transform->GetPosition() - cam->transform->GetPosition()).Normalized();
	//cam->transform->Translate(dir * 100 * Time::GetDeltaTime());

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

void Woo_Scene::Exit()
{
	// game object -> destroy()
	__super::Exit();
}

