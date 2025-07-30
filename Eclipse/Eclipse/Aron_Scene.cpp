#include "Aron_Scene.h"
#include "EclipseApp.h"
#include "Honmun.h"  // ���� ���� Honmun Ŭ���� ����
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Input.h"

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

	// [ ȥ�� enemies ] - ���ο� Honmun Ŭ���� ���
	honmun_a = CreateObject<Honmun>();
	honmun_a->name = "Honmun_A";
	honmun_a->SetHonmunType(HonmunType::A);
	honmun_a->SetPosition(-300.0f, 200.0f);

	honmun_b = CreateObject<Honmun>();
	honmun_b->name = "Honmun_B";
	honmun_b->SetHonmunType(HonmunType::B);
	honmun_b->SetPosition(-100.0f, 200.0f);

	honmun_c = CreateObject<Honmun>();
	honmun_c->name = "Honmun_C";
	honmun_c->SetHonmunType(HonmunType::C);
	honmun_c->SetPosition(100.0f, 200.0f);

	honmun_d = CreateObject<Honmun>();
	honmun_d->name = "Honmun_D";
	honmun_d->SetHonmunType(HonmunType::D);
	honmun_d->SetPosition(300.0f, 200.0f);

	// [ ground ] - ȥ������ �������� �ʵ���
	ground = CreateObject<GameObject>();
	ground->name = "Ground";
	ground->AddComponent<Transform>()->SetPosition(0.0f, -300.0f);
	auto ground_sr = ground->AddComponent<SpriteRenderer>();
	ground_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Ground.png"), "Ground");
	ground_sr->layer = 0; // ��׶��� ���̾�
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

	// �浹 ���� �� ���� ����
	CheckCollisionAndChangeColor();

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

	// AABB �׸��� - ������ (���� �� Honmun ������Ʈ���� ��ü������ ó��)
	if (ground_col) ground_col->DebugColliderDraw();

	OutputDebugStringA("�� ������Ʈ��");
}

void Aron_Scene::Exit()
{
	OutputDebugStringA("�� ��������� ��");
	// game object -> destroy()
	__super::Exit();
}

void Aron_Scene::CheckCollisionAndChangeColor()
{
	// �����̽��ٸ� ������ ��� ȥ���� �������ϰ� �����
	if (Input::GetKeyDown(VK_SPACE))
	{
		if (honmun_a) honmun_a->SetAlpha(0.5f);
		if (honmun_b) honmun_b->SetAlpha(0.5f);
		if (honmun_c) honmun_c->SetAlpha(0.5f);
		if (honmun_d) honmun_d->SetAlpha(0.5f);
	}

	// �����̽��ٸ� ���� ���� ������ ����
	if (Input::GetKeyUp(VK_SPACE))
	{
		if (honmun_a) honmun_a->ResetAlpha();
		if (honmun_b) honmun_b->ResetAlpha();
		if (honmun_c) honmun_c->ResetAlpha();
		if (honmun_d) honmun_d->ResetAlpha();
	}
}