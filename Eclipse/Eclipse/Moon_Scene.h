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

#include "HonA.h"
#include "HonB.h"
#include "HonC.h"
#include "HonD.h"
#include "Shadow.h"
#include "MapBackGround.h"

#include "Platform.h"

class Moon_Scene : public Scene
{
private:
	// [ background ]
	MapBackGround* mapBackGround;

	// [ player ]
	Player* player;						// Parent
	GameObject* playerAttack_Parent;	// Parent - Child
	PlayerAttackArea* playerAttackArea; // Parent - Child - Child 
	Shadow* shadow;

	// object 
	GameObject* cam;
	GameObject* ground;

	GameObject* mapEdge_l;
	GameObject* mapEdge_r;

	Platform_Short_Grass* platform_short1_grass;
	Platform_Short* platform_short2;
	Platform_Short* platform_short3;
	Platform_Short* platform_short4;
	Platform_Short* platform_short5;

	Platform_Middle* platform_middle1;
	Platform_Middle_Grass* platform_middle2_grass;
	Platform_Middle_Grass* platform_middle3_grass;
	Platform_Middle_Grass* platform_middle4_grass;
	Platform_Middle* platform_middle5;
	Platform_Middle* platform_middle6;

	Platform_Long_Grass* platform_long1_grass;
	Platform_Long_Grass* platform_long2_grass;

	Platform_Short_Grass* platform_short_grass1;
	Platform_Middle_Grass* platform_middle_grass1;
	Platform_Middle_Grass* platform_middle_grass2;
	Platform_Middle_Grass* platform_middle_grass3;
	Platform_Long_Grass* platform_long_grass1;

	BoxObject* box_object1;
	BoxObject* box_object2;
	BoxObject* box_object3;
	BoxObject* box_object4;

	Platform_Wall* platform_wall2;
	Platform_Wall_Grass* platform_wall1_grass;

	Cemetery* cemetery1;
	Cemetery* cemetery2;
	Cemetery* cemetery3;


	// [ UI ]
	 NPC* npc;
	 UI_Text* title_text;
	 PlayUI* playUI;

	// component 
	BoxCollider* ground_col;
	BoxCollider* wall_r_col;
	BoxCollider* wall_l_col;
	BoxCollider* platform1_col;
	BoxCollider* platform2_col;
	BoxCollider* playerAttack_col;

	BoxCollider* mapEdge_l_col;
	BoxCollider* mapEdge_r_col;

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
	Moon_Scene() = default;
	~Moon_Scene() = default;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Exit() override;
};

