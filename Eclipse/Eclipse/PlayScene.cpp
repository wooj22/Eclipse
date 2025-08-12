#include "PlayScene.h"
#include "EclipseApp.h"
#include "../Direct2D_EngineLib/SceneManager.h"

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
	// playerAttackArea->GetComponent<PlayerAreaController>()->SetPlayerFSM(player->GetComponent<PlayerFSM>());

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

}

void PlayScene::Start()
{
	// game object -> SceneStart()
	__super::Start();

	// start ambient wind sound
	if (ambientSoundManager)
	{
		ambientSoundManager->PlayWindSound();
	}
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
