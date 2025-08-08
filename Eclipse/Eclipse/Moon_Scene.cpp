#include "Moon_Scene.h"
#include "EclipseApp.h"

#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"

#include "Map_Woo.h"



void Moon_Scene::Awake()
{
	// [ camera ]
	cam = CreateObject<GameObject>();
	cam->AddComponent<Transform>()->SetPosition(-0.0f, -0.0f);
	auto camCompo = cam->AddComponent<Camera>(1920, 1080);

	// [BackGround Map]
	mapBackGround = CreateObject<MapBackGround>();

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

	// [ Platform ]
	platform_map = CreateObject<Platform>();
	
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

	playUI->quest_Image->SetActive(false);
	playUI->hon_Image->SetActive(false);
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
	// playerAttack_col->DebugColliderDraw();
}

void Moon_Scene::Exit()
{
	// game object -> destroy()
	__super::Exit();
}
