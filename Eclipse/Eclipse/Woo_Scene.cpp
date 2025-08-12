#include "Woo_Scene.h"
#include "EclipseApp.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/GameApp.h"

void Woo_Scene::Awake()
{
	// camera init
	cam = CreateObject<InGameCamera>();

	// gameobject
	// [Title Sample]
	title_text = CreateObject<UI_Text>();
	title_text->rectTransform->SetPosition(0, 500);
	title_text->rectTransform->SetSize(500, 100);
	title_text->screenTextRenderer->SetFontSize(50);
	title_text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::MediumPurple));
	title_text->screenTextRenderer->SetText(L"WooScene ´ö¿Â°øÁÖÃ¼");
	title_text->screenTextRenderer->SetFontName(L"´ö¿Â°øÁÖÃ¼");

	// [Boss]
	//boss = CreateObject<Boss>();

	// [Hon]
	honA1 = CreateObject<HonA>({ -400, -100 });
	/*honA2 = CreateObject<HonA>({ -500, -300 });
	honB1 = CreateObject<HonB>({ 0, 200 });
	honB2 = CreateObject<HonB>({ 200, -300 });*/
	honC1 = CreateObject<HonC>({ 0, 100 });
	honC2 = CreateObject<HonC>({ -200, 100 });
	//honC3 = CreateObject<HonC>({ -200, 200});
	/*honD1 = CreateObject<HonD>({ 100, 400 });
	honD2 = CreateObject<HonD>({ -100, 400 });*/

	// [BackGround Map]
	mapBackGround = CreateObject<MapBackGround>();

	// [Moon Shadow]
	moonShadow = CreateObject<MoonShadow>();
	moonShadow->transform->Translate(1000, 0);

	// [Fade Manager]
	fadeObject = CreateObject<FadeObject>();

	// [Ground Sample]
	ground = CreateObject<Map_Woo>({ 0, -800 });

	// slider engine test
	ui_slider = CreateObject<UI_Slider>();
	ui_slider->SetSliderSize(1000,30);
	ui_slider->SetHandleSize(40);

	auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Sample/Boss_Bullet.png");
	auto newSprite = ResourceManager::Get().CreateSprite(texture, "handle");
	ui_slider->SetHandleImage(newSprite);
	ui_slider->GetComponent<Slider>()->SetValue(0);

	ui_slider2 = CreateObject<UI_Slider>();
	ui_slider2->SetSliderSize(1000, 30);
	ui_slider2->SetHandleSize(40);
	ui_slider2->rectTransform->SetPosition(300, 300);
	ui_slider2->GetComponent<Slider>()->SetValue(1);

	// map border test
	GameObject* mapBorder = CreateObject<GameObject>({-500,0});
	mapBorder->AddComponent<Transform>();
	c =  mapBorder->AddComponent<BoxCollider>();
	Rigidbody* rb = mapBorder->AddComponent<Rigidbody>();

	mapBorder->name = "MapBorder";
	mapBorder->transform->SetPosition(-500, 0);
	rb->isKinematic = true;
	rb->useGravity = false;
	c->size = { 30, 1500 };
	c->isTrigger = true;

	/*--------------------------------------------------*/
	/* ¼±¹ÎÀÌ²¨ */
	// [ player ] 
	player = CreateObject<Player>();
	player->transform->SetPosition(0, 500);

	// [ playerAttack_Parent ]
	playerAttack_Parent = CreateObject<GameObject>();
	auto playerAttack_Parent_tr = playerAttack_Parent->AddComponent<Transform>();
	playerAttack_Parent_tr->SetParent(player->transform);
	playerAttack_Parent_tr->SetPosition(0.0f, 0.0f);
	player->playerFSM->SetPlayerAttackParent(playerAttack_Parent);

	// [ playerAttack ] Attack ÀÌÆåÆ® & ÄÝ¶óÀÌ´õ ¿µ¿ª 
	playerAttackArea = CreateObject<PlayerAttackArea>();
	playerAttackArea->GetComponent<Transform>()->SetParent(playerAttack_Parent->transform);
	playerAttackArea->SetActive(false);
	player->playerFSM->SetPlayerAttackArea(playerAttackArea); // ÇÃ·¹ÀÌ¾î FSM¿¡ ¿¬°á

	/* À±¸ðÇÜ²¨ */
	playUI = CreateObject<PlayUI>();
	GameManager::Get().g_playUI = playUI;
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

	// test
	c->DebugColliderDraw();

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

