#include "SkillWindowButton.h"

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

	skillIcon_Button->rectTransform->SetParent(this->rectTransform);
	skillLevel_Text->rectTransform->SetParent(this->rectTransform);
	skillName_Text->rectTransform->SetParent(this->rectTransform);
	skillDesc_Text->rectTransform->SetParent(this->rectTransform);
	skillColor1_Text->rectTransform->SetParent(this->rectTransform);
	skillColor2_Text->rectTransform->SetParent(this->rectTransform);
	skillColor3_Text->rectTransform->SetParent(this->rectTransform);

	skillIcon_Button->rectTransform->SetSize(100,100); //이미지 크기

}

void SkillWindowButton::SceneStart()
{

	const auto& skillInfo = GameManager::Get().skillTree[skillName];

	//이미지 및 폰트 크기에따라 미세 조정 필요
	skillLevel_Text->rectTransform->SetPosition(0,-50); 

	skillName_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
	skillName_Text->screenTextRenderer->SetFontSize(24);
	skillName_Text->rectTransform->SetSize(250, 50);
	skillName_Text->rectTransform->SetPosition(GetWidthSize(skillName_Text->rectTransform->GetSize(), skillIcon_Button->rectTransform->GetSize()), 50);

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

	skillName_Text->screenTextRenderer->SetText(skillInfo.skillname);
	skillDesc_Text->screenTextRenderer->SetText(skillInfo.skillDesc);


	if (skillInfo.maxLevel > 1)
	{
		skillColor1_Text->screenTextRenderer->SetText(ToWString(skillInfo.skillValue[0]));
		skillColor2_Text->screenTextRenderer->SetText(ToWString(skillInfo.skillValue[1]));
		if (skillInfo.skillValue.size() >= 3)
			skillColor3_Text->screenTextRenderer->SetText(ToWString(skillInfo.skillValue[2]));
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
}

void SkillWindowButton::Update()
{
	const auto& info = GameManager::Get().skillTree[skillName];

	if (info.unlockLevel != prevLevel)
	{
		prevLevel = info.unlockLevel;

		// 텍스트 갱신
		skillLevel_Text->screenTextRenderer->SetText(
			to_wstring(info.unlockLevel) + L"/" + to_wstring(maxSkillLevel));


		// 먼저 전부 초기화 (FloralWhite)
		if (prevLevel >= 1)
			skillColor1_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::FloralWhite));
		if (prevLevel >= 2)
			skillColor2_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::FloralWhite));
		if (prevLevel >= 3)
			skillColor3_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::FloralWhite));

		// 현재 레벨 칸만 빨간색
		if (prevLevel == 1)
			skillColor1_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
		else if (prevLevel == 2)
			skillColor2_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
		else if (prevLevel == 3)
			skillColor3_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
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
void SkillWindowButton::OnClickSkillButton()
{
	GameManager::Get().LevelUpSkill(skillName);
}