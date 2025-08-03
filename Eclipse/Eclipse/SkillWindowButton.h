#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/UI_Button.h"
#include "../Direct2D_EngineLib/UI_Image.h"
#include"../Direct2D_EngineLib/UI_Text.h"
#include"../Direct2D_EngineLib/Input.h"
#include"../Direct2D_EngineLib/SceneManager.h"
#include"../Direct2D_EngineLib/Scene.h"
#include"GameManager.h"


class SkillWindowButton : public GameObject
{
private: 
	int maxSkillLevel = 1;	// 스킬 최대 레벨
	SkillType skillName;		// 스킬 이름
	float colorPositionX = 0;

public:
	UI_Button* skillIcon_Button;	// 스킬 아이콘 버튼
	UI_Text* skillLevel_Text;		// 스킬 레벨 텍스트
	UI_Text* skillName_Text;		// 스킬 이름 텍스트
	UI_Text* skillDesc_Text;		// 스킬 설명 텍스트
	UI_Text* skillColor1_Text;		// 스킬 레벨 별 컬러1 텍스트
	UI_Text* skillColor2_Text;		// 스킬 레벨 별 컬러2 텍스트
	UI_Text* skillColor3_Text;		// 스킬 레벨 별 컬러3 텍스트

	SkillWindowButton(SkillType name);
	~SkillWindowButton() override {}

	void Awake() override {}
	void SceneStart() override;
	void Update() override;
	void Destroyed() override {}

	float GetWidthSize(D2D1_SIZE_F image, D2D1_SIZE_F text);
	std::wstring ToWString(float value);
	void OnClickSkillButton();
};

