#pragma once
#include "../Direct2D_EngineLib/UI_Button.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include "../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/UI_Slider.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/Scene.h"
#include <vector>

class Options : public GameObject
{
public:
	UI_Image* optionWindowBackGround_Image;			// 옵션 창 검은 화면 이미지
	UI_Image* optionWindowBackGroundGradient_Image;	// 옵션 창 그라데이션 이미지
	UI_Image* background_Image;
	UI_Button* close_Button;
	UI_Button* sound_Button;
	UI_Button* key_Button;

	UI_Image* soundBase_Image;
	UI_Text* soundMaster_Text;
	UI_Text* soundBGM_Text;
	UI_Text* soundSFX_Text;
	UI_Text* soundAMB_Text;
	UI_Slider* soundMaster_slider;
	UI_Slider* soundBGM_slider;
	UI_Slider* soundSFX_slider;
	UI_Slider* soundAMB_slider;

	UI_Image* controlKeyBase_Image;
	UI_Image* leftClick_Image;
	UI_Image* rightClick_Image;
	std::vector<UI_Image*> controlKeyImage;
	std::vector<UI_Text*> controlKeyTitles;
	std::vector<UI_Text*> controlKeyNames;
	std::vector<UI_Text*> controlKeyDescriptions;

public:
	Options();
	~Options() = default;
	void Awake() override;
	void SceneStart() override;
	void Update() override;
	void Destroyed() override;
};

