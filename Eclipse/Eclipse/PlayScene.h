#pragma once
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/UI_Text.h"
#include "InGameCamera.h"
#include "MapBackGround.h"
#include "MoonShadow.h"
#include "Player.h"
#include "PlayUI.h"
#include "Platform.h"
#include "playerAttackArea.h"
#include "WaveSystem.h"
#include "AmbientSoundManager.h"
#include "PlayerSkillEffcet.h"

class PlayScene : public Scene
{
private:
	// camera
	InGameCamera* camera;

	// background
	MapBackGround* mapBackGround;

	// moon shadow
	MoonShadow* moonShadow;

	// UI
	PlayUI* playUI;

	// player 
	Player* player;						// Parent
	GameObject* playerAttack_Parent;	// Parent - Child
	PlayerAttackArea* playerAttackArea; // Parent - Child - Child 
	Shadow* shadow;
	PlayerSkillEffect* skillEffect;		// Parent - Child

	// Platform 
	Platform* platform_map;
	
	// WaveSystem
	GameObject* waveSystemObj;
	WaveSystem* waveSystem;
	
	// Ambient Sound
	AmbientSoundManager* ambientSoundManager;

public:
	PlayScene() = default;
	~PlayScene() = default;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Exit() override;
};

