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
	cam->AddComponent<Transform>();
	auto camCompo = cam->AddComponent<Camera>(1920, 1080);

	// [BackGround Map]
	mapBackGround = CreateObject<MapBackGround>();

	// [ title ]
	title_text = CreateObject<UI_Text>();
	title_text->rectTransform->SetPosition(0, 500);
	title_text->rectTransform->SetSize(500, 100);
	title_text->screenTextRenderer->SetFontSize(50);
	title_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::LightPink));
	title_text->screenTextRenderer->SetText(L"Moon Scene");

	// [ player ] 
	player = CreateObject<Player>();

	// [ playerAttack_Parent ]
	playerAttack_Parent = CreateObject<GameObject>();
	auto playerAttack_Parent_tr = playerAttack_Parent->AddComponent<Transform>();
	playerAttack_Parent_tr->SetParent(player->transform);
	playerAttack_Parent_tr->SetPosition(0.0f, 0.0f);
	player->playerFSM->SetPlayerAttackParent(playerAttack_Parent);

	// [ playerAttack ] Attack ����Ʈ & �ݶ��̴� ���� 
	playerAttackArea = CreateObject<PlayerAttackArea>();
	playerAttackArea->GetComponent<Transform>()->SetParent(playerAttack_Parent->transform);
	playerAttackArea->GetComponent<SpriteRenderer>()->SetEnabled(false); 
	playerAttackArea->GetComponent<CircleCollider>()->SetEnabled(false); 

	player->playerFSM->SetPlayerAttackArea(playerAttackArea); // �÷��̾� FSM�� ����

	
	// [ ground ]
	ground = CreateObject<GameObject>();
	ground->name = "Ground"; 
	ground->tag = "Ground"; 
	ground->AddComponent<Transform>()->SetPosition(0.0f, -800.0f);

	auto ground_sr = ground->AddComponent<SpriteRenderer>();
	ground_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/long_platform_grass.png"), "Ground");
	ground_sr->layer = 0;

	ground_col = ground->AddComponent<BoxCollider>();
	ground_col->size = { 1920.0f, 90.0f };
	ground_col->isFlatform = true;



	// [ Platform ]
	platform_short1 = CreateObject<Platform_Short>();
	platform_short1->GetComponent<Transform>()->SetPosition(0.0f, -200.0f);






	
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
	honA1 = CreateObject<HonA>({ -200, 300 });
	honA2 = CreateObject<HonA>({ -500, 300 });
	honB1 = CreateObject<HonB>({ 0, 300 });
	honB2 = CreateObject<HonB>({ 200, 300 });
	honC1 = CreateObject<HonC>({ -100, 300 });
	honC2 = CreateObject<HonC>({ -200, 300 });
	honC3 = CreateObject<HonC>({ -200, 600});
	honD1 = CreateObject<HonD>({ 100, 800 });
	honD2 = CreateObject<HonD>({ -100, 800 });
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

	// AABB �׸��� 
	// ground_col->DebugColliderDraw();
	// wall_r_col->DebugColliderDraw();
	// wall_l_col->DebugColliderDraw();
	// platform1_col->DebugColliderDraw();
	// platform2_col->DebugColliderDraw();
	// playerAttack_col->DebugColliderDraw();
}

void Moon_Scene::Exit()
{
	// game object -> destroy()
	__super::Exit();
}
