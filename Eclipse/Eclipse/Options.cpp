#include "Options.h"

Options::Options() : GameObject("Optionts", "Optionts")
{
	rectTransform = AddComponent<RectTransform>();

	optionWindowBackGround_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	optionWindowBackGroundGradient_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	background_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	window_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	close_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	sound_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	key_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();

	optionWindowBackGround_Image->rectTransform->SetParent(this->rectTransform);
	optionWindowBackGroundGradient_Image->rectTransform->SetParent(this->rectTransform);
	background_Image->rectTransform->SetParent(this->rectTransform);
	window_Image->rectTransform->SetParent(this->rectTransform);
	close_Button->rectTransform->SetParent(this->rectTransform);
	sound_Button->rectTransform->SetParent(this->rectTransform);
	key_Button->rectTransform->SetParent(this->rectTransform);
}

void Options::Awake()
{
	close_Button->button->onClickListeners.AddListener(
		this, [this]() { this->SetActive(false); });
}

void Options::SceneStart()
{
	rectTransform->SetPosition(0, -150);
	optionWindowBackGround_Image->rectTransform->SetSize(1920, 1080);
	optionWindowBackGround_Image->rectTransform->SetPosition(0, 150);
	optionWindowBackGroundGradient_Image->rectTransform->SetSize(1920, 1080);
	optionWindowBackGroundGradient_Image->rectTransform->SetPosition(0, 150);
	auto optionWindowBackGroundImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/OptionWindowBackGround.png");
	optionWindowBackGround_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(optionWindowBackGroundImageTexture, "OptionWindowBackGround");
	auto optionWindowBackGroundGradientImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/OptionWindowBackGroundGradient.png");
	optionWindowBackGroundGradient_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(optionWindowBackGroundGradientImageTexture, "OptionWindowBackGroundGradient");
	background_Image->rectTransform->SetSize(850, 600);
	auto backImagenTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/OptionsBackGround.png");
	background_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(backImagenTexture, "OptionsBackGround");


	window_Image->rectTransform->SetPosition(0, -50);
	window_Image->rectTransform->SetSize(650, 400);
	window_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(backImagenTexture, "OptionsBackGround");
	window_Image->imageRenderer->layer = 20;

	close_Button->rectTransform->SetPosition(310, 230);
	close_Button->rectTransform->SetSize(150, 100);
	auto closeButtonTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/BackButton.png");
	close_Button->imageRenderer->sprite = ResourceManager::Get().CreateSprite(closeButtonTexture, "BackButton");

	sound_Button->rectTransform->SetPosition(-270, 200);
	sound_Button->rectTransform->SetSize(200, 100);
	sound_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink, 0));

	key_Button->rectTransform->SetPosition(-70, 200);
	key_Button->rectTransform->SetSize(200, 100);
	key_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink, 0));

	//TODOMO : 이미지로하면 텍스트는 제거
	sound_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	sound_Button->screenTextRenderer->SetText(L"소리");
	sound_Button->screenTextRenderer->SetFontSize(60);
	sound_Button->screenTextRenderer->SetFontName(L"덕온공주체");

	key_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	key_Button->screenTextRenderer->SetText(L"조작");
	key_Button->screenTextRenderer->SetFontSize(60);
	key_Button->screenTextRenderer->SetFontName(L"덕온공주체");

}

void Options::Update()
{
}

void Options::Destroyed()
{
}
