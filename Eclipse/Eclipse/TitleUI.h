#pragma once
#include "../Direct2D_EngineLib/UI_Button.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include"../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "Options.h"

class TitleUI : public GameObject
{
public:
	UI_Image* backgroundImage;
	UI_Button* play_Button;
	UI_Button* options_Button;
	UI_Button* credit_Button;
	UI_Button* end_Button;
	UI_Image* underscore_Image;
	std::vector<UI_Button*> menuButtons;

	Options* optionUI;

	// hover event check
	UI_Button* currentHoverButton = nullptr;

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
	void OnPointEnterButton(UI_Button* parent);
	void OnPointExitButton(UI_Button* prevButton);
	void OpenOptionUI();
};



