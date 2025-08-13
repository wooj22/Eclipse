#include "tooltip.h"
#include <string>
#include <locale>
#include <codecvt>

static std::string ToString(const std::wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(wstr);
}

void tooltip::Awake()
{
	rectTransform = AddComponent<RectTransform>();
	tooltipBackGround_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	tooltip_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	tooltipName_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	tooltipDescribe_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();

	tooltipBackGround_Image->rectTransform->SetParent(rectTransform);
	tooltip_Image->rectTransform->SetParent(tooltipBackGround_Image->rectTransform);
	tooltipName_Text->rectTransform->SetParent(tooltipBackGround_Image->rectTransform);
	tooltipDescribe_Text->rectTransform->SetParent(tooltipBackGround_Image->rectTransform);

	textInfo.clear();

	//툴팁 텍스트 정보 초기화
	textInfo[L"Ignis"] = {L"이그니스", L"같은 영혼과\n충돌하면\n합쳐지며 커진다."};
	textInfo[L"Umbra"] = { L"엄브라", L"같은 영혼과\n충돌하면\n둘로 분리된다." };
	textInfo[L"Dark"] = { L"다크", L"같은 영혼과\n충돌하면\n주변을 끌어당긴다." };
	textInfo[L"Luna"] = {L"루나", L"파괴될 경우\n혼을 잃는다."};
	textInfo[L"Nox"] = {L"녹스", L"영혼을 충돌시켜\n대미지를 줄 수 있다.\n땅에 닿는 순간,\n모든 것이 끝난다."};		

	SetActive(false);
}

void tooltip::SceneStart()
{
	tooltipBackGround_Image->rectTransform->SetSize(300, 200);
	auto toolTipTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/ToolTip.png");
	tooltipBackGround_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(toolTipTexture, "ToolTip");

	tooltip_Image->rectTransform->SetPosition(-80,25);
	tooltip_Image->rectTransform->SetSize(80, 80);

	tooltipName_Text->rectTransform->SetPosition(-80, -50);
	tooltipName_Text->rectTransform->SetSize(90, 50);
	tooltipName_Text->screenTextRenderer->SetFontSize(30);
	tooltipName_Text->screenTextRenderer->SetFontName(L"덕온공주체");
	tooltipName_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));

	tooltipDescribe_Text->rectTransform->SetPosition(45, 0);
	tooltipDescribe_Text->rectTransform->SetSize(160, 50);
	tooltipDescribe_Text->screenTextRenderer->SetFontSize(17);
	tooltipDescribe_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));

}

void tooltip::Update()
{
}

void tooltip::Destroyed()
{
	textInfo.clear();
}

void tooltip::ChangeInfo(const std::wstring& name)
{
	if (name == L"Ignis") rectTransform->SetPosition(775, -200);
	else if (name == L"Umbra") rectTransform->SetPosition(775, -400);
	else rectTransform->SetPosition(775 , -300);

	auto it = textInfo.find(name);
	if (it != textInfo.end())
	{
		tooltipName_Text->screenTextRenderer->SetText(it->second.skillname);
		tooltipDescribe_Text->screenTextRenderer->SetText(it->second.skillDesc); 
		std::wstring fullPathW = L"../Resource/mo/" + name + L"_Tooltip.png";
		auto tooltipImageTexture = ResourceManager::Get().CreateTexture2D(ToString(fullPathW));
		tooltip_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(tooltipImageTexture, ToString(fullPathW));
		SetActive(true);
	}
	else
	{
		tooltipName_Text->screenTextRenderer->SetText(L"");
		tooltipDescribe_Text->screenTextRenderer->SetText(L"");
		SetActive(false);
	}
}