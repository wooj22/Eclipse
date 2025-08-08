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

#include "Platform_Short.h"
#include "Platform_Middle.h"
#include "Platform_Long.h"
#include "Platform_Short_Grass.h"
#include "Platform_Middle_Grass.h"
#include "Platform_Long_Grass.h"
#include "BoxObject.h"
#include "Platform_Wall.h"
#include "Platform_Wall_Grass.h"
#include "Cemetery.h"

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

	Platform_Short* platform_short1;
	Platform_Short* platform_short2;
	Platform_Short* platform_short3;
	Platform_Short* platform_short4;
	Platform_Short* platform_short5;

	Platform_Middle* platform_middle1;
	Platform_Middle* platform_middle2;
	Platform_Middle* platform_middle3;
	Platform_Middle* platform_middle4;
	Platform_Middle* platform_middle5;
	Platform_Middle* platform_middle6;

	Platform_Long* platform_long1;
	Platform_Long* platform_long2;

	Platform_Short_Grass* platform_short_grass1;
	Platform_Middle_Grass* platform_middle_grass1;
	Platform_Middle_Grass* platform_middle_grass2;
	Platform_Middle_Grass* platform_middle_grass3;
	Platform_Long_Grass* platform_long_grass1;

	BoxObject* box_object1;
	BoxObject* box_object2;
	BoxObject* box_object3;
	BoxObject* box_object4;

	Platform_Wall* platform_wall1;
	Platform_Wall* platform_wall2;
	Platform_Wall_Grass* platform_wall_grass1;

	Cemetery* cemetery1;
	Cemetery* cemetery2;

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

