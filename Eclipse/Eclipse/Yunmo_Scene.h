#pragma once
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/UI_Text.h"

#include "Player.h"
#include "playerAttackArea.h"
#include "NPC.h"

#include "PlayUI.h"
#include "SkillWindowButton.h"
#include "WaveSystem.h"

#include "HonA.h"
#include "HonB.h"
#include "HonC.h"
#include "HonD.h"

class Yunmo_Scene : public Scene
{
private:
	// 일부 오브젝트는 현재 씬에서 모두 처리 중. 추후 스크립트 할당 예정 
	GameObject* cam;
	UI_Text* title_text;

	GameObject* ground;
	GameObject* wall_r;
	GameObject* wall_l;
	GameObject* platform1;
	GameObject* platform2;
	GameObject* platform3;
	GameObject* platform4;

	Player* player;						// Parent
	GameObject* playerAttack_Parent;	// Parent - Child
	PlayerAttackArea* playerAttackArea; // Parent - Child - Child 


	// [ UI ]
	NPC* npc;
	PlayUI* playUI;

	// [ Wave ] 
	GameObject* waveSystemObj;
	WaveSystem* waveSystem;

	// component 
	BoxCollider* ground_col;
	BoxCollider* wall_r_col;
	BoxCollider* wall_l_col;
	BoxCollider* platform1_col;
	BoxCollider* platform2_col;
	BoxCollider* platform3_col;
	BoxCollider* platform4_col;
	BoxCollider* playerAttack_col;

	// Hon
	HonA* honA1;
	HonA* honA2;
	HonB* honB1;
	HonB* honB2;
	HonC* honC1;
	HonC* honC2;
	HonC* honC3;
	HonD* honD1;
	HonD* honD2;

public:
	Yunmo_Scene() = default;
	~Yunmo_Scene() = default;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Exit() override;
};

