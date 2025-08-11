#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include "../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/UI_Button.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/Scene.h"


class Credit : public GameObject
{
public:
	UI_Image* creditWindowBackGround_Image;
	UI_Image* creditWindowBackGroundGradient_Image;
	UI_Image* background_Image;
	UI_Image* splitter_Image;
	UI_Button* close_Button;
	UI_Text* creditTitle_Text;
	UI_Text* plannerTitle_Text;
	UI_Text* artTitle_Text;
	UI_Text* programmerTitle_Text;
	UI_Text* plannerName_Text;
	UI_Text* artName_Text;
	UI_Text* programmerName_Text;

public:
	void Awake() override;
	void SceneStart() override;
	void Update() override;
	void Destroyed() override;

};
