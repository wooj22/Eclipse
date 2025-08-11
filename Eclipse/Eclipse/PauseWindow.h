#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include "../Direct2D_EngineLib/UI_Button.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/Scene.h"

class PauseWindow : public GameObject
{
public:
	UI_Image* optionWindowBackGround_Image;
	UI_Image* optionWindowBackGroundGradient_Image;
	UI_Image* window_Image;
	UI_Button* close_Button;
	UI_Button* newGame_Button;
	UI_Button* continuGame_Button;
	UI_Button* quitGame_Button;

	UI_Image* underscore_Image;
	UI_Button* prevHoverButton = nullptr;
	std::vector<UI_Button*> eventButtons;

public:
	void Awake() override;
	void SceneStart() override;   
	void Update() override;
	void OnPointEnterButton(UI_Button* parent);
	void OnPointExitButton(UI_Button* prevButton);
};

