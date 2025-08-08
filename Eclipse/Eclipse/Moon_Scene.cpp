#include "Moon_Scene.h"
#include "EclipseApp.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "Map_Woo.h"
#include "Platform_Short.h"

void Moon_Scene::Awake()
{
	// [ camera ]
	cam = CreateObject<GameObject>();
	cam->AddComponent<Transform>()->SetPosition(-0.0f, -0.0f);
	auto camCompo = cam->AddComponent<Camera>(1920, 1080);

	// [BackGround Map]
	mapBackGround = CreateObject<MapBackGround>();

	// [ title ]
	//title_text = CreateObject<UI_Text>();
	//title_text->rectTransform->SetPosition(0, 500);
	//title_text->rectTransform->SetSize(500, 100);
	//title_text->screenTextRenderer->SetFontSize(50);
	//title_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::LightPink));
	//title_text->screenTextRenderer->SetText(L"Moon Scene");

	// [ player ] 
	player = CreateObject<Player>();

	// [ playerAttack_Parent ]
	playerAttack_Parent = CreateObject<GameObject>();
	auto playerAttack_Parent_tr = playerAttack_Parent->AddComponent<Transform>();
	playerAttack_Parent_tr->SetParent(player->transform);
	playerAttack_Parent_tr->SetPosition(0.0f, 0.0f);
	player->playerFSM->SetPlayerAttackParent(playerAttack_Parent);

	// [ playerAttack ] Attack 이펙트 & 콜라이더 영역 
	playerAttackArea = CreateObject<PlayerAttackArea>();
	playerAttackArea->GetComponent<Transform>()->SetParent(playerAttack_Parent->transform);
	playerAttackArea->GetComponent<SpriteRenderer>()->SetEnabled(false); 
	playerAttackArea->GetComponent<CircleCollider>()->SetEnabled(false); 

	player->playerFSM->SetPlayerAttackArea(playerAttackArea); // 플레이어 FSM에 연결

	// [ ground ]
	ground = CreateObject<GameObject>();
	ground->name = "Ground"; 
	ground->tag = "Ground"; 
	ground->AddComponent<Transform>()->SetPosition(0.0f, -1000.0f);

	ground_col = ground->AddComponent<BoxCollider>();
	ground_col->size = { 2560.0f, 300.0f };
	ground_col->isFlatform = true;


	// 맵 바깥 경계 
	mapEdge_l = CreateObject<GameObject>();
	mapEdge_l->AddComponent<Transform>()->SetPosition(-1280.0f - 150.0f, 0.0f);
	mapEdge_l_col = mapEdge_l->AddComponent<BoxCollider>();
	mapEdge_l_col->size = { 300.0f, 1920.0f };

	mapEdge_r = CreateObject<GameObject>();
	mapEdge_r->AddComponent<Transform>()->SetPosition(1280.0f + 150.0f, 0.0f);
	mapEdge_r_col = mapEdge_r->AddComponent<BoxCollider>();
	mapEdge_r_col->size = { 300.0f, 1920.0f };



	// [ Platform ] : 왼쪽 하단부터

	platform_long1 = CreateObject<Platform_Long>();
	platform_long1->GetComponent<Transform>()->SetPosition(-800.0f, -600.0f);

	platform_middle1 = CreateObject<Platform_Middle>();
    platform_middle1->GetComponent<Transform>()->SetPosition(350.0f, -600.0f);

	platform_short1 = CreateObject<Platform_Short>();
	platform_short1->GetComponent<Transform>()->SetPosition(-150.0f, -450.0f);

	platform_middle2 = CreateObject<Platform_Middle>();
	platform_middle2->GetComponent<Transform>()->SetPosition(850.0f, -450.0f);

	platform_middle3 = CreateObject<Platform_Middle>();
	platform_middle3->GetComponent<Transform>()->SetPosition(-600.0f, -280.0f);

	platform_middle4 = CreateObject<Platform_Middle>();
	platform_middle4->GetComponent<Transform>()->SetPosition(250.0f, -280.0f);

	platform_short2 = CreateObject<Platform_Short>();
	platform_short2->GetComponent<Transform>()->SetPosition(750.0f, -120.0f);

	platform_short3 = CreateObject<Platform_Short>();
	platform_short3->GetComponent<Transform>()->SetPosition(-1000.0f, -50.0f);

	platform_short4 = CreateObject<Platform_Short>();
	platform_short4->GetComponent<Transform>()->SetPosition(300.0f, 30.0f);

	platform_middle5 = CreateObject<Platform_Middle>();
	platform_middle5->GetComponent<Transform>()->SetPosition(-550.0f, 120.0f);

	platform_long2 = CreateObject<Platform_Long>();
	platform_long2->GetComponent<Transform>()->SetPosition(950.0f, 120.0f);

	platform_short5 = CreateObject<Platform_Short>();
	platform_short5->GetComponent<Transform>()->SetPosition(-850.0f, 350.0f);

	platform_middle6 = CreateObject<Platform_Middle>();
	platform_middle6->GetComponent<Transform>()->SetPosition(500.0f, 350.0f);


	//platform_short_grass1 = CreateObject<Platform_Short_Grass>();
	//platform_short_grass1->GetComponent<Transform>()->SetPosition(-400.0f, -400.0f);

	//platform_middle_grass1 = CreateObject<Platform_Middle_Grass>();
	//platform_middle_grass1->GetComponent<Transform>()->SetPosition(0.0f, -400.0f);

	//platform_long_grass1 = CreateObject<Platform_Long_Grass>();
	//platform_long_grass1->GetComponent<Transform>()->SetPosition(600.0f, -100.0f);


	
	// boundary condition
	Rect mapRect;
	mapRect.size = { 2560, 1920 };

	// camera target
	camCompo->SetTarget(player->transform);
	camCompo->SetTargetTraceSpeed(200.0f);
	camCompo->SetTargetTraceLimitX(30.0f);
	camCompo->SetTargetTraceLimitY(100.0f);
	camCompo->SetMapCondition(mapRect);


	// [ UI ] 
	playUI = CreateObject<PlayUI>();
	GameManager::Get().g_playUI = playUI;


	// [ Hon ]
	//honA1 = CreateObject<HonA>({ -200, 300 });
	//honA2 = CreateObject<HonA>({ -500, 300 });
	//honB1 = CreateObject<HonB>({ 0, 300 });
	//honB2 = CreateObject<HonB>({ 200, 300 });
	//honC1 = CreateObject<HonC>({ -100, 300 });
	//honC2 = CreateObject<HonC>({ -200, 300 });
	//honC3 = CreateObject<HonC>({ -200, 600});
	//honD1 = CreateObject<HonD>({ 100, 800 });
	//honD2 = CreateObject<HonD>({ -100, 800 });
}

void Moon_Scene::Start()
{
	// game object -> SceneStart()
	__super::Start();

}

void Moon_Scene::Update()
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

	// AABB 그리기 
	ground_col->DebugColliderDraw();
	mapEdge_l_col->DebugColliderDraw();
	mapEdge_r_col->DebugColliderDraw();
	// platform1_col->DebugColliderDraw();
	// platform2_col->DebugColliderDraw();
	// playerAttack_col->DebugColliderDraw();
}

void Moon_Scene::Exit()
{
	// game object -> destroy()
	__super::Exit();
}
