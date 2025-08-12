#include "Yunmo_Scene.h"
#include "EclipseApp.h"
#include "../Direct2D_EngineLib/SceneManager.h"

void Yunmo_Scene::Awake()
{
	// [ camera ]
	cam = CreateObject<GameObject>();
	cam->AddComponent<Transform>();
	auto camCompo = cam->AddComponent<Camera>(1920, 1080);

	// [ BackGround ]
	auto backGround = CreateObject<GameObject>();
	backGround->AddComponent<Transform>()->SetPosition(0.0f, 0.0f);;
	auto background_sr = backGround->AddComponent<SpriteRenderer>();
	background_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Background_1.png"), "Background_Moon_1");


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
	ground->tag = "EndLine";
	ground->AddComponent<Transform>()->SetPosition(0.0f, -900.0f);

	auto ground_sr = ground->AddComponent<SpriteRenderer>();
	ground_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Ground.png"), "Ground");
	ground_sr->layer = 0;

	ground_col = ground->AddComponent<BoxCollider>();
	ground_col->size = { 2560.0f, 50.0f };
	ground_col->isFlatform = true;


	// [ wall_r ]
	wall_r = CreateObject<GameObject>();
	wall_r->name = "Wall";
	wall_r->AddComponent<Transform>()->SetPosition(1280.0f, -300.0f);;

	auto wall_r_sr = wall_r->AddComponent<SpriteRenderer>();
	wall_r_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Wall.png"), "Wall");

	wall_r_col = wall_r->AddComponent<BoxCollider>();
	wall_r_col->size = { 50.0f, 1400.0f };


	// [ wall_l ]
	wall_l = CreateObject<GameObject>();
	wall_l->name = "Wall";
	wall_l->AddComponent<Transform>()->SetPosition(-1280.0f, -300.0f);;

	auto wall_l_sr = wall_l->AddComponent<SpriteRenderer>();
	wall_l_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Wall.png"), "Wall");

	wall_l_col = wall_l->AddComponent<BoxCollider>();
	wall_l_col->size = { 50.0f, 1400.0f };

	// [ Platform1 ]
	platform1 = CreateObject<GameObject>();
	platform1->name = "Ground";
	platform1->AddComponent<Transform>()->SetPosition(200.0f, -750.0f);

	auto platform1_sr = platform1->AddComponent<SpriteRenderer>();
	platform1_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Platform.png"), "Platform");

	platform1_col = platform1->AddComponent<BoxCollider>();
	platform1_col->offset = { 0.0f, 12.0f };
	platform1_col->size = { 200.0f, 5.0f };
	platform1_col->isFlatform = true;

	// [ Platform2 ]
	platform2 = CreateObject<GameObject>();
	platform2->name = "Ground";
	platform2->AddComponent<Transform>()->SetPosition(550.0f, -600.0f);

	auto platform2_sr = platform2->AddComponent<SpriteRenderer>();
	platform2_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Platform.png"), "Platform");

	platform2_col = platform2->AddComponent<BoxCollider>();
	platform2_col->size = { 200.0f, 30.0f };
	platform2_col->isFlatform = true;

	// [ Platform3 ]
	platform3 = CreateObject<GameObject>();
	platform3->name = "Ground";
	platform3->AddComponent<Transform>()->SetPosition(-200.0f, -750.0f);

	auto platform3_sr = platform3->AddComponent<SpriteRenderer>();
	platform3_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Platform.png"), "Platform");

	platform3_col = platform3->AddComponent<BoxCollider>();
	platform3_col->size = { 200.0f, 30.0f };
	platform3_col->isFlatform = true;

	// [ Platform4 ]
	platform4 = CreateObject<GameObject>();
	platform4->name = "Ground";
	platform4->AddComponent<Transform>()->SetPosition(-550.0f, -600.0f);

	auto platform4_sr = platform4->AddComponent<SpriteRenderer>();
	platform4_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Platform.png"), "Platform");

	platform4_col = platform4->AddComponent<BoxCollider>();
	platform4_col->size = { 200.0f, 30.0f };
	platform4_col->isFlatform = true;

	// boundary condition
	Rect mapRect;
	mapRect.size = { 2560, 1920 };

	// camera target
	camCompo->SetTarget(player->transform);
	camCompo->SetTargetTraceXSpeed(200.0f);
	camCompo->SetTargetTraceLimitX(30.0f);
	camCompo->SetTargetTraceLimitY(100.0f);
	camCompo->SetMapCondition(mapRect);

	//wave
	waveSystemObj = CreateObject<GameObject>();
	waveSystemObj->name = "WaveSystem";
	waveSystemObj->SetActive(true);  // GameObject 활성화
	waveSystem = waveSystemObj->AddComponent<WaveSystem>();

	npc = CreateObject<NPC>({0,-800});


	playUI = CreateObject<PlayUI>();
	GameManager::Get().g_playUI = playUI;
}

void Yunmo_Scene::Start()
{
	// game object -> SceneStart()
	__super::Start();

}

void Yunmo_Scene::Update()
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

	if (Input::GetKeyDown('C'))
	{
		GameManager::Get().AllSkillUnlock();
	}

	if (Input::GetKeyDown('R'))
	{
		GameManager::Get().SkillReset();
	}

	// AABB 그리기 
	ground_col->DebugColliderDraw();
	wall_r_col->DebugColliderDraw();
	wall_l_col->DebugColliderDraw();
}

void Yunmo_Scene::Exit()
{
	// game object -> destroy()
	__super::Exit();
}
