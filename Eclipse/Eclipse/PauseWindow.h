#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include "../Direct2D_EngineLib/UI_Button.h"
#include "../Direct2D_EngineLib/UI_Slider.h"
#include "../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "LineClip.h"

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

	UI_Image* soundBase_Image;
	UI_Text* soundMaster_Text;
	UI_Text* soundBGM_Text;
	UI_Text* soundSFX_Text;
	UI_Text* soundAMB_Text;
	UI_Slider* soundMaster_slider;
	UI_Slider* soundBGM_slider;
	UI_Slider* soundSFX_slider;
	UI_Slider* soundAMB_slider;

	UI_Image* underscore_Image;
	LineController* linecontroller;

	UI_Button* prevHoverButton = nullptr;
	std::vector<UI_Button*> eventButtons;

public:
	void Awake() override;
	void SceneStart() override;   
	void Update() override;
	void Destroyed() override;
	void OnPointEnterButton(UI_Button* parent);
	void OnPointExitButton(UI_Button* prevButton);
};

