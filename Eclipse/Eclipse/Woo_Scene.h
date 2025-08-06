#pragma once
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include "../Direct2D_EngineLib/UI_Button.h"
#include "Player_Woo.h"
#include "Map_Woo.h"
#include "Boss.h"
#include "HonA.h"
#include "HonB.h"
#include "HonC.h"
#include "HonD.h"

#include "Player.h"
#include "playerAttackArea.h"

#include "NPC.h"
#include "SkillWindowButton.h"
#include "PlayUI.h"


class Woo_Scene : public Scene
{
private:
	GameObject* cam;
	UI_Text* title_text;
	GameObject* map;
	Map_Woo* map_woo;
	Map_Woo* ground1;
	Map_Woo* ground2;
	Map_Woo* ground3;
	
	// Boss
	Boss* boss;

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

	// UI
	NPC* npc;
	PlayUI* playUI;

	// player sample
	//Player_Woo* player;

	// rect parent pivot test
	/*UI_Image* parent;
	UI_Image* child;*/

	// button new event test
	//UI_Button* button;

	/* 선민이 오브젝트 */
	GameObject* wall_r;
	GameObject* wall_l;

	Player* player;						// Parent
	GameObject* playerAttack_Parent;	// Parent - Child
	PlayerAttackArea* playerAttackArea; // Parent - Child - Child 

	// component 
	BoxCollider* wall_r_col;
	BoxCollider* wall_l_col;
	BoxCollider* playerAttack_col;


public:
	Woo_Scene() = default;
	~Woo_Scene() = default;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Exit() override;
};

