#pragma once
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/UI_Text.h"

// player
#include "Player.h"
#include "playerAttackArea.h"

// honmun
#include "Honmun.h"
#include "HonmunCollisionScript.h"
#include "HonmunAFSM.h"
#include "HonmunAAnimatorController.h"
#include "HonmunBFSM.h"
#include "HonmunBAnimatorController.h"
#include "HonmunCFSM.h"
#include "HonmunCAnimatorController.h"

class Yunmo_Scene : public Scene
{
private:
	GameObject* cam;
	UI_Text* title_text;

	GameObject* ground;
	GameObject* wall_r;
	GameObject* wall_l;
	GameObject* platform1;
	GameObject* platform2;

	Player* player;						// Parent
	GameObject* playerAttack_Parent;	// Parent - Child
	PlayerAttackArea* playerAttackArea; // Parent - Child - Child 

	// component 
	BoxCollider* ground_col;
	BoxCollider* wall_r_col;
	BoxCollider* wall_l_col;
	BoxCollider* platform1_col;
	BoxCollider* platform2_col;
	BoxCollider* playerAttack_col;

	// honmun
	Honmun* honmun_a;
	Honmun* honmun_a2;
	Honmun* honmun_a3;
	Honmun* honmun_a4;
	Honmun* honmun_b;
	Honmun* honmun_b2;
	Honmun* honmun_b3;
	Honmun* honmun_b4;



public:
	Yunmo_Scene() = default;
	~Yunmo_Scene() = default;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Exit() override;
};

