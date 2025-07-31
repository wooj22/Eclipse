#pragma once
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include "Player_Woo.h"
#include "Map_Woo.h"
#include "Boss.h"

class Woo_Scene : public Scene
{
private:
	GameObject* cam;
	UI_Text* title_text;
	Player_Woo* player;
	Boss* boss;
	GameObject* map;
	Map_Woo* map_woo;
	Map_Woo* ground1;
	Map_Woo* ground2;
	Map_Woo* ground3;

	// rect parent pivot test
	/*UI_Image* parent;
	UI_Image* child;*/

public:
	Woo_Scene() = default;
	~Woo_Scene() = default;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Exit() override;
};

