#pragma once
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/UI_Text.h"

#include "Player.h"
#include "playerAttackArea.h"

class Moon_Scene : public Scene
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


public:
	Moon_Scene() = default;
	~Moon_Scene() = default;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Exit() override;
};

