#pragma once
#include "../Direct2D_EngineLib/UI_Button.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include"../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/GameObject.h"

class TitleUI : public GameObject
{
public:
	UI_Image* backgroundImage;
	//UI_Text* titleText;
	UI_Button* playButton;
	UI_Button* optionsButton;
	UI_Button* creditButton;
	UI_Button* endButton;

public:
	// game object cycle
	TitleUI() {}
	~TitleUI() override {}

	// 오브젝트가 생성될 때
	void Awake() override;

	// Scene의 Start (씬의 모든 오브젝트가 생성되고 난 뒤)
	void SceneStart() override;

	// Scene의 Update
	void Update()  override;

	// Scene의 Exit, GameObject Delete
	void Destroyed() override;
};



