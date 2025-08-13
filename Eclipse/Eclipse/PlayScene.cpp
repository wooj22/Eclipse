#include "PlayScene.h"
#include "EclipseApp.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "PerformanceChecker.h"

void PlayScene::Awake()
{
	// camera
	camera = CreateObject<InGameCamera>();

	// create object
	mapBackGround = CreateObject<MapBackGround>();
	moonShadow = CreateObject<MoonShadow>();
	moonShadow->transform->Translate(1000, 0);

	// UI
	playUI = CreateObject<PlayUI>();
	GameManager::Get().g_playUI = playUI;

	// [ player ] 
	player = CreateObject<Player>();

	// [ player SkillEffect ]
	skillEffect = CreateObject<PlayerSkillEffect>();
	skillEffect->GetComponent<Transform>()->SetParent(player->transform);
	player->playerFSM->SetPlayerSkillEffect(skillEffect);

	// [ Platform ]
	platform_map = CreateObject<Platform>();

	//wave
	waveSystemObj = CreateObject<GameObject>();
	waveSystemObj->name = "WaveSystem";
	waveSystemObj->SetActive(true);  // GameObject 활성화
	waveSystem = waveSystemObj->AddComponent<WaveSystem>();
	
	// ambient sound
	ambientSoundManager = CreateObject<AmbientSoundManager>();

	// fade object
	fadeObject = CreateObject<FadeObject>();

	// performace cheker
	CreateObject<PerformanceChecker>();
}

void PlayScene::Start()
{
	// game object -> SceneStart()
	__super::Start();

	// Ambient sounds will play periodically through the AmbientSoundManager's Update method
}

void PlayScene::Update()
{
	// game object -> Update()
	__super::Update();

	//if (Input::GetKeyDown('P')) {
	//	mapBackGround->Destroy();
	//}
}

void PlayScene::Exit()
{
	// game object -> destroy()
	GameManager::Get().ReSetData();
	__super::Exit();
}
