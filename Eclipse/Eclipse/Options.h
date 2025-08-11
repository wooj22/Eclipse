#pragma once
#include "../Direct2D_EngineLib/UI_Button.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include "../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/Scene.h"

class Options : public GameObject
{
public:
	UI_Image* optionWindowBackGround_Image;			// 옵션 창 검은 화면 이미지
	UI_Image* optionWindowBackGroundGradient_Image;	// 옵션 창 그라데이션 이미지
	UI_Image* background_Image;
	UI_Button* close_Button;
	UI_Button* sound_Button;
	UI_Button* key_Button;
public:
	Options();
	~Options() = default;
	void Awake() override;
	void SceneStart() override;
	void Update() override;
	void Destroyed() override;
};

