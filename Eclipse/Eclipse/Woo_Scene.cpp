#include "Woo_Scene.h"
#include "EclipseApp.h"
#include "../Direct2D_EngineLib/SceneManager.h"

void Woo_Scene::Awake()
{
	// camera init
	cam = CreateObject<GameObject>();
	cam->AddComponent<Transform>();
	cam->AddComponent<Camera>(1920, 1080);

	// gameobject
	// [Title Sample]
	title_text = CreateObject<UI_Text>();
	title_text->rectTransform->SetPosition(0, 500);
	title_text->rectTransform->SetSize(500, 100);
	title_text->screenTextRenderer->SetFontSize(50);
	title_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::MediumPurple));
	title_text->screenTextRenderer->SetText(L"WooScene 덕온공주체");
	title_text->screenTextRenderer->SetFontName(L"덕온공주체");

	// [Boss]
	//boss = CreateObject<Boss>();

	// [Hon]
	//honA1 = CreateObject<HonA>({ -200, -500 });
	//honA2 = CreateObject<HonA>({ -500, -300 });
	honB1 = CreateObject<HonB>({ 0, 200 });
	//honB2 = CreateObject<HonB>({ 200, -300 });
	honC1 = CreateObject<HonC>({ 0, -400 });
	//honC2 = CreateObject<HonC>({ -200, -100 });
	//honC3 = CreateObject<HonC>({ -200, 200});
	//honD1 = CreateObject<HonD>({ 100, 400 });
	//honD2 = CreateObject<HonD>({ -100, 400 });


	// [BackGround Map]
	mapBackGround = CreateObject<MapBackGround>();
	mapBackGround->backGround00_Sky = CreateObject<BackGround00_Sky>();
	mapBackGround->backGround01_Moon = CreateObject<BackGround01_Moon>();
	mapBackGround->backGround02_Mount = CreateObject<BackGround02_Mount>();
	mapBackGround->backGround03_Tree = CreateObject<BackGround03_Tree>();

	// [Ground Sample]
	ground = CreateObject<Map_Woo>({ 0, -900 });


	/*--------------------------------------------------*/
	/* 선민이꺼 */
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
	playerAttackArea->SetActive(false);
	player->playerFSM->SetPlayerAttackArea(playerAttackArea); // 플레이어 FSM에 연결

	//// [ wall_r ]
	//wall_r = CreateObject<GameObject>();
	//wall_r->name = "Wall";
	//wall_r->AddComponent<Transform>()->SetPosition(850.0f, -500.0f);;

	//auto wall_r_sr = wall_r->AddComponent<SpriteRenderer>();
	//wall_r_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Wall.png"), "Wall");

	//wall_r_col = wall_r->AddComponent<BoxCollider>();
	//wall_r_col->size = { 30.0f, 750.0f };

	//// [ wall_l ]
	//wall_l = CreateObject<GameObject>();
	//wall_l->name = "Wall";
	//wall_l->AddComponent<Transform>()->SetPosition(-850.0f, -500.0f);;

	//auto wall_l_sr = wall_l->AddComponent<SpriteRenderer>();
	//wall_l_sr->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Wall.png"), "Wall");

	//wall_l_col = wall_l->AddComponent<BoxCollider>();
	//wall_l_col->size = { 30.0f, 750.0f };

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

	/* 윤모햄꺼 */
	npc = CreateObject<NPC>({ -600, -700 });

	playUI = CreateObject<PlayUI>();
	GameManager::Get().g_playUI = playUI;
	playUI->timer_Text = CreateObject<UI_Text>();
	playUI->stop_Button = CreateObject<UI_Button>();
	playUI->quest_Image = CreateObject<UI_Image>();
	playUI->quest_Text = CreateObject<UI_Text>();
	playUI->chat_Image = CreateObject<UI_Image>();
	playUI->chat_Text = CreateObject<UI_Text>();
	playUI->chat_Button = CreateObject<UI_Button>();
	playUI->hon_Image = CreateObject<UI_Image>();
	playUI->hon_Text = CreateObject<UI_Text>();
	playUI->skill1_Image = CreateObject<UI_Image>();
	playUI->skill1_Text = CreateObject<UI_Text>();
	playUI->skill2_Image = CreateObject<UI_Image>();
	playUI->waveInfo_Text = CreateObject<UI_Text>();
	playUI->tooltip_Image = CreateObject<UI_Image>();

	playUI->skillWindow_Image = CreateObject<UI_Image>();
	playUI->skillButtons.push_back(CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::KnockbackDistanceUp));
	playUI->skillButtons.push_back(CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::DoubleJump));
	playUI->skillButtons.push_back(CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::WallJump));
	playUI->skillButtons.push_back(CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::SkillCooldownDown));
	playUI->skillButtons.push_back(CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::JumpAttackExtra));
	playUI->skillButtons.push_back(CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::FastFall));
	playUI->skillButtons.push_back(CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::MoveSpeedUp));
	playUI->skillButtons.push_back(CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::AttackRangeUp));
	playUI->skillButtons.push_back(CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::Dash));
	playUI->skillHon_Image = CreateObject<UI_Image>();
	playUI->skillHon_Text = CreateObject<UI_Text>();
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

	// AABB 그리기 
	//wall_r_col->DebugColliderDraw();
	//wall_l_col->DebugColliderDraw();
}

void Woo_Scene::Exit()
{
	// game object -> destroy()
	__super::Exit();
}

