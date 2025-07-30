#include "TitleUI.h"

void TitleUI::Awake()
{
}

void TitleUI::SceneStart()
{
	backgroundImage->rectTransform->SetPivot(0.5, 0.5);
	backgroundImage->rectTransform->SetSize(1920, 1080);
	auto title = ResourceManager::Get().CreateTexture2D("../Resource/mo/TitleBackGround.png");
	backgroundImage->imageRenderer->sprite = ResourceManager::Get().CreateSprite(title, "TitleBackGround");

	playButton->rectTransform->SetPosition(0, 100);
	playButton->rectTransform->SetSize(200, 100);
	auto playButtonTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/PlayButton.png");
	playButton->imageRenderer->sprite = ResourceManager::Get().CreateSprite(playButtonTexture, "PlayButton");

	optionsButton->rectTransform->SetPosition(0, 0);
	optionsButton->rectTransform->SetSize(200, 100);
	optionsButton->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink));
	optionsButton->screenTextRenderer->SetText(L"옵션");
	optionsButton->screenTextRenderer->SetFontSize(30);

	creditButton->rectTransform->SetPosition(0, -100);
	creditButton->rectTransform->SetSize(200, 100);
	creditButton->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink));
	creditButton->screenTextRenderer->SetText(L"크레딧");
	creditButton->screenTextRenderer->SetFontSize(30);

	endButton->rectTransform->SetPosition(0, -200);
	endButton->rectTransform->SetSize(200, 100);
	endButton->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink));
	endButton->screenTextRenderer->SetText(L"게임 종료");
	endButton->screenTextRenderer->SetFontSize(30);
}

void TitleUI::Update()
{

}

void TitleUI::Destroyed()
{

}