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

	// [ ȥ�� enemy a ]
	honmun_a = CreateObject<GameObject>();
	honmun_a->name = "Honmun_A";
	honmun_a->AddComponent<Transform>()->SetPosition(-300.0f, 200.0f); // �� ���� ��ġ���� ��������
	//honmun_a->transform->SetScale(0.15f, 0.15f);
	honmun_a->transform->SetScale(1, 1);
	honmun_a_sr = honmun_a->AddComponent<SpriteRenderer>();
	honmun_a_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Honmun_a.png"), "Honmun_A");
	//honmun_a_sr->layer = 1; // ���̾ ������ �����ͼ� �ݶ��̴��� ���̰�
	honmun_a_col = honmun_a->AddComponent<BoxCollider>();
	honmun_a_col->size = { 100.0f, 100.0f }; // ���� �� ũ�� �ؼ� ������ ���̰�
	auto honmun_a_rb = honmun_a->AddComponent<Rigidbody>();
	honmun_a_rb->useGravity = true;
	honmun_a_rb->isKinematic = false;

	// [ ȥ�� enemy b ]
	honmun_b = CreateObject<GameObject>();
	honmun_b->name = "Honmun_B";
	honmun_b->AddComponent<Transform>()->SetPosition(-100.0f, 200.0f); // �� ���� ��ġ���� ��������
	honmun_b->transform->SetScale(1, 1);
	honmun_b_sr = honmun_b->AddComponent<SpriteRenderer>();
	honmun_b_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Honmun_b.png"), "Honmun_B");
	//honmun_b_sr->layer = 1; // ���̾ ������ �����ͼ� �ݶ��̴��� ���̰�
	honmun_b_col = honmun_b->AddComponent<BoxCollider>();
	honmun_b_col->size = { 100.0f, 100.0f }; // ���� �� ũ�� �ؼ� ������ ���̰�
	auto honmun_b_rb = honmun_b->AddComponent<Rigidbody>();
	honmun_b_rb->useGravity = true;
	honmun_b_rb->isKinematic = false;

	// [ ȥ�� enemy c ]
	honmun_c = CreateObject<GameObject>();
	honmun_c->name = "Honmun_C";
	honmun_c->AddComponent<Transform>()->SetPosition(100.0f, 200.0f); // �� ���� ��ġ���� ��������
	honmun_c->transform->SetScale(1, 1);
	honmun_c_sr = honmun_c->AddComponent<SpriteRenderer>();
	honmun_c_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Honmun_c.png"), "Honmun_C");
	//honmun_c_sr->layer = 1; // ���̾ ������ �����ͼ� �ݶ��̴��� ���̰�
	honmun_c_col = honmun_c->AddComponent<BoxCollider>();
	honmun_c_col->size = { 100.0f, 100.0f }; // ���� �� ũ�� �ؼ� ������ ���̰�
	auto honmun_c_rb = honmun_c->AddComponent<Rigidbody>();
	honmun_c_rb->useGravity = true;
	honmun_c_rb->isKinematic = false;

	// [ ȥ�� enemy d ]
	honmun_d = CreateObject<GameObject>();
	honmun_d->name = "Honmun_D";
	honmun_d->AddComponent<Transform>()->SetPosition(300.0f, 200.0f); // �� ���� ��ġ���� ��������
	honmun_d->transform->SetScale(1, 1);
	honmun_d_sr = honmun_d->AddComponent<SpriteRenderer>();
	honmun_d_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Aron/Honmun_d.png"), "Honmun_D");
	//honmun_d_sr->layer = 1; // ���̾ ������ �����ͼ� �ݶ��̴��� ���̰�
	honmun_d_col = honmun_d->AddComponent<BoxCollider>();
	honmun_d_col->size = { 100.0f, 100.0f }; // ���� �� ũ�� �ؼ� ������ ���̰�
	auto honmun_d_rb = honmun_d->AddComponent<Rigidbody>();
	honmun_d_rb->useGravity = true;
	honmun_d_rb->isKinematic = false;

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

	// AABB �׸��� - ������
	/*if (ground_col) ground_col->DebugColliderDraw();
	if (honmun_a_col) honmun_a_col->DebugColliderDraw();
	if (honmun_b_col) honmun_b_col->DebugColliderDraw();
	if (honmun_c_col) honmun_c_col->DebugColliderDraw();
	if (honmun_d_col) honmun_d_col->DebugColliderDraw();*/

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
	// ������ ����: �����̽��ٸ� ������ ��� ȥ���� �������ϰ� ����� (�浹 ȿ��)
	if (Input::GetKeyDown(VK_SPACE))
	{
		if (honmun_a_sr) honmun_a_sr->alpha = 0.5f; // ������
		if (honmun_b_sr) honmun_b_sr->alpha = 0.5f;
		if (honmun_c_sr) honmun_c_sr->alpha = 0.5f;
		if (honmun_d_sr) honmun_d_sr->alpha = 0.5f;
	}

	// RŰ�� ������ ���� ������ ����
	if (Input::GetKeyUp(VK_SPACE))
	{
		if (honmun_a_sr) honmun_a_sr->alpha = 1.0f; // ���� ������
		if (honmun_b_sr) honmun_b_sr->alpha = 1.0f;
		if (honmun_c_sr) honmun_c_sr->alpha = 1.0f;
		if (honmun_d_sr) honmun_d_sr->alpha = 1.0f;
	}
}