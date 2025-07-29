#include "Aron_Scene.h"
#include "EclipseApp.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"

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

	// [ 혼문 enemy a ]
	honmun_a = CreateObject<GameObject>();
	honmun_a->name = "Honmun_A";
	honmun_a->AddComponent<Transform>()->SetPosition(-300.0f, 200.0f); // 더 높은 위치에서 떨어지게
	//honmun_a->transform->SetScale(0.15f, 0.15f);
	honmun_a->transform->SetScale(1, 1);
	honmun_a_sr = honmun_a->AddComponent<SpriteRenderer>();
	honmun_a_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Honmun_a.png"), "Honmun_A");
	//honmun_a_sr->layer = 1; // 레이어를 앞으로 가져와서 콜라이더가 보이게
	honmun_a_col = honmun_a->AddComponent<BoxCollider>();
	honmun_a_col->size = { 100.0f, 100.0f }; // 조금 더 크게 해서 밖으로 보이게
	auto honmun_a_rb = honmun_a->AddComponent<Rigidbody>();
	honmun_a_rb->useGravity = true;
	honmun_a_rb->isKinematic = false;

	// [ 혼문 enemy b ]
	honmun_b = CreateObject<GameObject>();
	honmun_b->name = "Honmun_B";
	honmun_b->AddComponent<Transform>()->SetPosition(-100.0f, 200.0f); // 더 높은 위치에서 떨어지게
	honmun_b->transform->SetScale(1, 1);
	honmun_b_sr = honmun_b->AddComponent<SpriteRenderer>();
	honmun_b_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Honmun_b.png"), "Honmun_B");
	//honmun_b_sr->layer = 1; // 레이어를 앞으로 가져와서 콜라이더가 보이게
	honmun_b_col = honmun_b->AddComponent<BoxCollider>();
	honmun_b_col->size = { 100.0f, 100.0f }; // 조금 더 크게 해서 밖으로 보이게
	auto honmun_b_rb = honmun_b->AddComponent<Rigidbody>();
	honmun_b_rb->useGravity = true;
	honmun_b_rb->isKinematic = false;

	// [ 혼문 enemy c ]
	honmun_c = CreateObject<GameObject>();
	honmun_c->name = "Honmun_C";
	honmun_c->AddComponent<Transform>()->SetPosition(100.0f, 200.0f); // 더 높은 위치에서 떨어지게
	honmun_c->transform->SetScale(1, 1);
	honmun_c_sr = honmun_c->AddComponent<SpriteRenderer>();
	honmun_c_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Honmun_c.png"), "Honmun_C");
	//honmun_c_sr->layer = 1; // 레이어를 앞으로 가져와서 콜라이더가 보이게
	honmun_c_col = honmun_c->AddComponent<BoxCollider>();
	honmun_c_col->size = { 100.0f, 100.0f }; // 조금 더 크게 해서 밖으로 보이게
	auto honmun_c_rb = honmun_c->AddComponent<Rigidbody>();
	honmun_c_rb->useGravity = true;
	honmun_c_rb->isKinematic = false;

	// [ 혼문 enemy d ]
	honmun_d = CreateObject<GameObject>();
	honmun_d->name = "Honmun_D";
	honmun_d->AddComponent<Transform>()->SetPosition(300.0f, 200.0f); // 더 높은 위치에서 떨어지게
	honmun_d->transform->SetScale(1, 1);
	honmun_d_sr = honmun_d->AddComponent<SpriteRenderer>();
	honmun_d_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Honmun_d.png"), "Honmun_D");
	//honmun_d_sr->layer = 1; // 레이어를 앞으로 가져와서 콜라이더가 보이게
	honmun_d_col = honmun_d->AddComponent<BoxCollider>();
	honmun_d_col->size = { 100.0f, 100.0f }; // 조금 더 크게 해서 밖으로 보이게
	auto honmun_d_rb = honmun_d->AddComponent<Rigidbody>();
	honmun_d_rb->useGravity = true;
	honmun_d_rb->isKinematic = false;

	// [ ground ] - 혼문들이 떨어지지 않도록
	ground = CreateObject<GameObject>();
	ground->name = "Ground";
	ground->AddComponent<Transform>()->SetPosition(0.0f, -300.0f);
	auto ground_sr = ground->AddComponent<SpriteRenderer>();
	ground_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Ground.png"), "Ground");
	ground_sr->layer = 0; // 백그라운드 레이어
	ground_col = ground->AddComponent<BoxCollider>();
	ground_col->size = { 1110.0f, 30.0f };
	auto ground_rb = ground->AddComponent<Rigidbody>();
	ground_rb->useGravity = false;
	ground_rb->isKinematic = true;
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

	// 충돌 감지 및 색상 변경
	//CheckCollisionAndChangeColor();

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

	// AABB 그리기 - 디버깅용
	/*if (ground_col) ground_col->DebugColliderDraw();
	if (honmun_a_col) honmun_a_col->DebugColliderDraw();
	if (honmun_b_col) honmun_b_col->DebugColliderDraw();
	if (honmun_c_col) honmun_c_col->DebugColliderDraw();
	if (honmun_d_col) honmun_d_col->DebugColliderDraw();*/

	OutputDebugStringA("씬 업데이트중");
}

void Aron_Scene::Exit()
{
	OutputDebugStringA("씬 종료까지는 옴");
	// game object -> destroy()
	__super::Exit();

	
}

void Aron_Scene::CheckCollisionAndChangeColor()
{
	// 간단한 예시: 스페이스바를 누르면 모든 혼문을 반투명하게 만들기 (충돌 효과)
	if (Input::GetKeyDown(VK_SPACE))
	{
		if (honmun_a_sr) honmun_a_sr->alpha = 0.5f; // 반투명
		if (honmun_b_sr) honmun_b_sr->alpha = 0.5f;
		if (honmun_c_sr) honmun_c_sr->alpha = 0.5f;
		if (honmun_d_sr) honmun_d_sr->alpha = 0.5f;
	}

	// R키를 누르면 원래 투명도로 복원
	if (Input::GetKeyUp(VK_SPACE))
	{
		if (honmun_a_sr) honmun_a_sr->alpha = 1.0f; // 완전 불투명
		if (honmun_b_sr) honmun_b_sr->alpha = 1.0f;
		if (honmun_c_sr) honmun_c_sr->alpha = 1.0f;
		if (honmun_d_sr) honmun_d_sr->alpha = 1.0f;
	}
}