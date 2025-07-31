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

	// [ \ud63c\ubb38 enemies ] - \uc6e8\uc774\ube0c 1 \ud14c\uc2a4\ud2b8\uc6a9 A, B \uc544\uc774\ud15c \uc5ec\ub7ec \uac1c \uc0dd\uc131
	// A \ud0c0\uc785 \ud63c\ubb38 4\uac1c
	honmun_a = CreateObject<Honmun>();
	honmun_a->name = "Honmun_A1";
	honmun_a->SetHonmunType(HonmunType::A);
	honmun_a->SetPosition(-300.0f, 200.0f);

	auto* honmun_a2 = CreateObject<Honmun>();
	honmun_a2->name = "Honmun_A2";
	honmun_a2->SetHonmunType(HonmunType::A);
	honmun_a2->SetPosition(-200.0f, 300.0f);

	auto* honmun_a3 = CreateObject<Honmun>();
	honmun_a3->name = "Honmun_A3";
	honmun_a3->SetHonmunType(HonmunType::A);
	honmun_a3->SetPosition(-400.0f, 100.0f);

	auto* honmun_a4 = CreateObject<Honmun>();
	honmun_a4->name = "Honmun_A4";
	honmun_a4->SetHonmunType(HonmunType::A);
	honmun_a4->SetPosition(-100.0f, 150.0f);

	// B \ud0c0\uc785 \ud63c\ubb38 4\uac1c (더 넓게 배치)
	honmun_b = CreateObject<Honmun>();
	honmun_b->name = "Honmun_B1";
	honmun_b->SetHonmunType(HonmunType::B);
	honmun_b->SetPosition(150.0f, 250.0f);

	auto* honmun_b2 = CreateObject<Honmun>();
	honmun_b2->name = "Honmun_B2";
	honmun_b2->SetHonmunType(HonmunType::B);
	honmun_b2->SetPosition(350.0f, 400.0f);

	auto* honmun_b3 = CreateObject<Honmun>();
	honmun_b3->name = "Honmun_B3";
	honmun_b3->SetHonmunType(HonmunType::B);
	honmun_b3->SetPosition(450.0f, 50.0f);

	auto* honmun_b4 = CreateObject<Honmun>();
	honmun_b4->name = "Honmun_B4";
	honmun_b4->SetHonmunType(HonmunType::B);
	honmun_b4->SetPosition(50.0f, 50.0f);

	// 모든 혼문을 벡터에 추가
	allHonmuns.push_back(honmun_a);
	allHonmuns.push_back(honmun_a2);
	allHonmuns.push_back(honmun_a3);
	allHonmuns.push_back(honmun_a4);
	allHonmuns.push_back(honmun_b);
	allHonmuns.push_back(honmun_b2);
	allHonmuns.push_back(honmun_b3);
	allHonmuns.push_back(honmun_b4);

	// 웨이브 1 테스트: C, D 주석처리
	// honmun_c = CreateObject<Honmun>();
	// honmun_c->name = "Honmun_C";
	// honmun_c->SetHonmunType(HonmunType::C);
	// honmun_c->SetPosition(100.0f, 200.0f);

	// honmun_d = CreateObject<Honmun>();
	// honmun_d->name = "Honmun_D";
	// honmun_d->SetHonmunType(HonmunType::D);
	// honmun_d->SetPosition(300.0f, 200.0f);

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

	// 충돌 테스트 및 색상 변경
	CheckCollisionAndChangeColor();
	
	// 혼문 이동 컨트롤
	HandleHonmunMovement();

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
		// 웨이브 1 테스트: C, D 주석처리
		// if (honmun_c) honmun_c->SetAlpha(0.5f);
		// if (honmun_d) honmun_d->SetAlpha(0.5f);
	}

	// �����̽��ٸ� ���� ���� �������� ����
	if (Input::GetKeyUp(VK_SPACE))
	{
		if (honmun_a) honmun_a->ResetAlpha();
		if (honmun_b) honmun_b->ResetAlpha();
		// 웨이브 1 테스트: C, D 주석처리
		// if (honmun_c) honmun_c->ResetAlpha();
		// if (honmun_d) honmun_d->ResetAlpha();
	}
}

void Aron_Scene::HandleHonmunMovement()
{
	float moveSpeed = 3.0f; // 키네마틱 이동 속도
	
	// Q/E 키로 오브젝트 선택
	if (Input::GetKeyDown('Q'))
	{
		selectedHonmunIndex = (selectedHonmunIndex - 1 + allHonmuns.size()) % allHonmuns.size();
		// 유효하지 않은 오브젝트는 건너뛰기
		while (selectedHonmunIndex < allHonmuns.size() && 
			   (!allHonmuns[selectedHonmunIndex] || !allHonmuns[selectedHonmunIndex]->IsActive()))
		{
			selectedHonmunIndex = (selectedHonmunIndex - 1 + allHonmuns.size()) % allHonmuns.size();
		}
		char debugMsg[100];
		sprintf_s(debugMsg, "Selected Honmun index: %d\n", selectedHonmunIndex);
		OutputDebugStringA(debugMsg);
	}
	if (Input::GetKeyDown('E'))
	{
		selectedHonmunIndex = (selectedHonmunIndex + 1) % allHonmuns.size();
		// 유효하지 않은 오브젝트는 건너뛰기
		while (selectedHonmunIndex < allHonmuns.size() && 
			   (!allHonmuns[selectedHonmunIndex] || !allHonmuns[selectedHonmunIndex]->IsActive()))
		{
			selectedHonmunIndex = (selectedHonmunIndex + 1) % allHonmuns.size();
		}
		char debugMsg[100];
		sprintf_s(debugMsg, "Selected Honmun index: %d\n", selectedHonmunIndex);
		OutputDebugStringA(debugMsg);
	}
	
	// 선택된 오브젝트 이동 (화살표 키)
	if (selectedHonmunIndex < allHonmuns.size() && 
		allHonmuns[selectedHonmunIndex] && 
		allHonmuns[selectedHonmunIndex]->IsActive())
	{
		auto* transform = allHonmuns[selectedHonmunIndex]->GetComponent<Transform>();
		if (transform)
		{
			Vector2 currentPos = transform->GetPosition();
			if (Input::GetKey(VK_LEFT)) transform->SetPosition(currentPos.x - moveSpeed, currentPos.y);
			if (Input::GetKey(VK_RIGHT)) transform->SetPosition(currentPos.x + moveSpeed, currentPos.y);
			if (Input::GetKey(VK_UP)) transform->SetPosition(currentPos.x, currentPos.y + moveSpeed);
			if (Input::GetKey(VK_DOWN)) transform->SetPosition(currentPos.x, currentPos.y - moveSpeed);
		}
	}
	
	// 웨이브 1 테스트: C, D 주석처리
	// // C 혼문 (IJKL로 상하좌우 이동)
	// if (honmun_c)
	// {
	// 	auto* rb = honmun_c->GetComponent<Rigidbody>();
	// 	if (rb)
	// 	{
	// 		if (Input::GetKey('J')) rb->AddForce(Vector2(-moveSpeed, 0));
	// 		if (Input::GetKey('L')) rb->AddForce(Vector2(moveSpeed, 0));
	// 		if (Input::GetKey('I')) rb->AddForce(Vector2(0, moveSpeed));
	// 		if (Input::GetKey('K')) rb->AddForce(Vector2(0, -moveSpeed));
	// 	}
	// }
	// 
	// // D 혼문 (8456 숫자키로 상하좌우 이동)
	// if (honmun_d)
	// {
	// 	auto* rb = honmun_d->GetComponent<Rigidbody>();
	// 	if (rb)
	// 	{
	// 		if (Input::GetKey('4')) rb->AddForce(Vector2(-moveSpeed, 0));
	// 		if (Input::GetKey('6')) rb->AddForce(Vector2(moveSpeed, 0));
	// 		if (Input::GetKey('8')) rb->AddForce(Vector2(0, moveSpeed));
	// 		if (Input::GetKey('5')) rb->AddForce(Vector2(0, -moveSpeed));
	// 	}
	// }
	
	// R - 모든 혼문 리셋 (원래 위치로)
	if (Input::GetKeyDown('R'))
	{
		if (honmun_a) honmun_a->SetPosition(-300.0f, 200.0f);
		if (honmun_b) honmun_b->SetPosition(-100.0f, 200.0f);
		// 웨이브 1 테스트: C, D 주석처리
		// if (honmun_c) honmun_c->SetPosition(100.0f, 200.0f);
		// if (honmun_d) honmun_d->SetPosition(300.0f, 200.0f);
	}
}