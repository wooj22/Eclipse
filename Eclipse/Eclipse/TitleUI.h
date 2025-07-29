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

	// ������Ʈ�� ������ ��
	void Awake() override;

	// Scene�� Start (���� ��� ������Ʈ�� �����ǰ� �� ��)
	void SceneStart() override;

	// Scene�� Update
	void Update()  override;

	// Scene�� Exit, GameObject Delete
	void Destroyed() override;
};



