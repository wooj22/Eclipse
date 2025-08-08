#include "SkillWindowButton.h"
#include <DirectXMath.h>

SkillWindowButton::SkillWindowButton(SkillType name) : GameObject("SkillWindowButton", "SkillWindowButton"), skillName(name)
{
	rectTransform = AddComponent<RectTransform>();
	maxSkillLevel = GameManager::Get().skillTree[skillName].maxLevel;
	skillIcon_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	skillLevel_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	skillName_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	skillDesc_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	skillColor1_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	skillColor2_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	skillColor3_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	skillLevelUpHonCount_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();

	skillIcon_Button->rectTransform->SetParent(this->rectTransform);
	skillLevel_Text->rectTransform->SetParent(this->rectTransform);
	skillName_Text->rectTransform->SetParent(this->rectTransform);
	skillDesc_Text->rectTransform->SetParent(this->rectTransform);
	skillColor1_Text->rectTransform->SetParent(this->rectTransform);
	skillColor2_Text->rectTransform->SetParent(this->rectTransform);
	skillColor3_Text->rectTransform->SetParent(this->rectTransform);
	skillLevelUpHonCount_Text->rectTransform->SetParent(this->rectTransform);

	skillIcon_Button->rectTransform->SetSize(100,100); //이미지 크기
	skillIcon_Button->imageRenderer->renderMode = RenderMode::UnlitColorTint;
	skillIcon_Button->imageRenderer->SetColor(0.4, 0.4, 0.4);
	RefreshCurrentLevelText();
}

void SkillWindowButton::SceneStart()
{
	const auto& skillInfo = GameManager::Get().skillTree[skillName];
	const auto& skillvalue = GameManager::Get().skillValue[skillName];
	const auto& skilltext = GameManager::Get().skillText[skillName];


	//이미지 및 폰트 크기에따라 미세 조정 필요
	skillLevel_Text->rectTransform->SetPosition(0,-50); 

	skillName_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
	skillName_Text->screenTextRenderer->SetFontSize(24);
	skillName_Text->rectTransform->SetSize(250, 50);
	skillName_Text->rectTransform->SetPosition(GetWidthSize(skillName_Text->rectTransform->GetSize(), skillIcon_Button->rectTransform->GetSize()), 50);
	skillName_Text->screenTextRenderer->SetFontName(L"덕온공주체");

	skillDesc_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
	skillDesc_Text->screenTextRenderer->SetVerticalAlign(TextVerticalAlign::Top);
	skillDesc_Text->rectTransform->SetSize(300,50);
	skillDesc_Text->rectTransform->SetPosition(GetWidthSize(skillDesc_Text->rectTransform->GetSize(), skillIcon_Button->rectTransform->GetSize()), 0);

	skillColor1_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
	skillColor2_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Center);
	skillColor3_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Right);
	skillColor1_Text->rectTransform->SetSize(100, 50);
	colorPositionX = GetWidthSize(skillColor1_Text->rectTransform->GetSize(), skillIcon_Button->rectTransform->GetSize())+10;
	skillColor1_Text->rectTransform->SetPosition(colorPositionX, -10);
	skillColor2_Text->rectTransform->SetPosition(colorPositionX, -10);
	skillColor3_Text->rectTransform->SetPosition(colorPositionX, -10);

	skillName_Text->screenTextRenderer->SetText(skilltext.skillname);
	skillDesc_Text->screenTextRenderer->SetText(skilltext.skillDesc);

	skillLevelUpHonCount_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
	skillLevelUpHonCount_Text->rectTransform->SetSize(300, 50);
	skillLevelUpHonCount_Text->rectTransform->SetPosition(GetWidthSize(skillLevelUpHonCount_Text->rectTransform->GetSize(), skillIcon_Button->rectTransform->GetSize()), -65);

	skillLevelUpHonCount_Text->screenTextRenderer->SetText(skilltext.skillname);
	RefreshLevelUPHonCountText();

	if (skillName == SkillType::SkillCooldownDown)
	{
		skillColor1_Text->screenTextRenderer->SetText(ToWString(skillvalue[0]));
		skillColor2_Text->screenTextRenderer->SetText(ToWString(skillvalue[1]));
		skillColor3_Text->SetActive(false);
	}
	else if (skillInfo.maxLevel > 1)
	{
		skillColor1_Text->screenTextRenderer->SetText(ToWString(static_cast<int>(std::round((skillvalue[0] - 1) * 100))));
		skillColor2_Text->screenTextRenderer->SetText(ToWString(static_cast<int>(std::round((skillvalue[1] - 1) * 100))));
		skillColor3_Text->screenTextRenderer->SetText(ToWString(static_cast<int>(std::round((skillvalue[2] - 1) * 100))));
	}
	else
	{
		//혹시 모를 비활성화
		skillColor1_Text->SetActive(false);
		skillColor2_Text->SetActive(false);
		skillColor3_Text->SetActive(false);
	}

	skillIcon_Button->button->onClickListeners.AddListener(
		this, std::bind(&SkillWindowButton::OnClickSkillButton, this));

	skillIcon_Button->button->onPointEnterListeners.AddListener(
		this, std::bind(&SkillWindowButton::OnPointEnterButton, this));

	skillIcon_Button->button->onPointExitListeners.AddListener(
		this, std::bind(&SkillWindowButton::OnPointExitButton, this));
}

void SkillWindowButton::Update()
{
	if (isEnterButton)
	{
		glowtimer += Time::GetDeltaTime();  // 매 프레임 시간 누적

		float glow = ((sinf(glowtimer * glowspeed - DirectX::XM_PIDIV2)+1.0f) / 2.0f) * 30.0f;
		skillIcon_Button->imageRenderer->SetGlowAmmount(glow);
	}
}

float SkillWindowButton::GetWidthSize(D2D1_SIZE_F size1, D2D1_SIZE_F size2)
{
	return (size1.width + size2.width) / 2;
}

std::wstring SkillWindowButton::ToWString(float value)
{
	if (value == static_cast<int>(value))
	{
		return std::to_wstring(static_cast<int>(value));
	}
	else
	{
		std::wstringstream wss;
		wss << std::fixed << std::setprecision(1) << value;
		return wss.str();
	}
}

void SkillWindowButton::RefreshCurrentLevelText()
{
	const auto& skillinfo = GameManager::Get().skillTree[skillName];
	// 텍스트 갱신
	skillLevel_Text->screenTextRenderer->SetText(
		to_wstring(skillinfo.unlockLevel) + L"/" + to_wstring(maxSkillLevel));

	// 먼저 전부 초기화 (FloralWhite)
	skillColor1_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::FloralWhite));
	skillColor2_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::FloralWhite));
	skillColor3_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::FloralWhite));

	// 현재 레벨 칸만 빨간색
	if (skillinfo.unlockLevel == 1)
		skillColor1_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
	else if (skillinfo.unlockLevel == 2)
		skillColor2_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
	else if (skillinfo.unlockLevel == 3)
		skillColor3_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
}

void SkillWindowButton::RefreshLevelUPHonCountText()
{
	const auto& skillInfo = GameManager::Get().skillTree[skillName];
	if (skillInfo.unlockLevel != skillInfo.maxLevel)
		skillLevelUpHonCount_Text->screenTextRenderer->SetText(L"필요 혼 : " + to_wstring(skillInfo.skillCost[skillInfo.unlockLevel]));
	else
		skillLevelUpHonCount_Text->screenTextRenderer->SetText(L"");
}

void SkillWindowButton::OnClickSkillButton()
{
	if (GameManager::Get().LevelUpSkill(skillName))
	{
		// 해금 또는 레벨업 성공 후 다시한번 레벨업 가능한지 체크
		if (GameManager::Get().LevelUpSkill(skillName, true))
			isEnterButton = true;
		else
			skillIcon_Button->imageRenderer->renderMode = RenderMode::Unlit;

		RefreshCurrentLevelText();
		RefreshLevelUPHonCountText();
	}
}

void SkillWindowButton::OnPointEnterButton()
{
	// 해금 또는 레벨업 가능 체크
	if (GameManager::Get().CanUnlock(skillName) || GameManager::Get().LevelUpSkill(skillName,true))
	{
		skillIcon_Button->imageRenderer->renderMode = RenderMode::Lit_Glow;
		isEnterButton = true;
		glowtimer = 0;
	}
}

void SkillWindowButton::OnPointExitButton()
{
	if (isEnterButton)
	{
		// 해금이 안된 경우 UnlitColorTint로 변경
		if(GameManager::Get().CanUnlock(skillName))
			skillIcon_Button->imageRenderer->renderMode = RenderMode::UnlitColorTint;
		else
			skillIcon_Button->imageRenderer->renderMode = RenderMode::Unlit;

		isEnterButton = false;
	}
}