#include "Options.h"

Options::Options() : GameObject("Optionts", "Optionts")
{
	rectTransform = AddComponent<RectTransform>();

	background_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	window_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	close_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	sound_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	key_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();

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
	background_Image->rectTransform->SetSize(1248, 702);
	auto backImagenTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/OptionsBackGround.png");
	background_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(backImagenTexture, "OptionsBackGround");


	window_Image->rectTransform->SetSize(811, 456);
	window_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(backImagenTexture, "OptionsBackGround");
	window_Image->imageRenderer->layer = 20;

	close_Button->rectTransform->SetPosition(500, 280);
	close_Button->rectTransform->SetSize(150, 100);
	auto closeButtonTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/BackButton.png");
	close_Button->imageRenderer->sprite = ResourceManager::Get().CreateSprite(closeButtonTexture, "BackButton");

	sound_Button->rectTransform->SetPosition(-475, 200);
	sound_Button->rectTransform->SetSize(200, 100);
	sound_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink, 0));

	key_Button->rectTransform->SetPosition(-475, 100);
	key_Button->rectTransform->SetSize(200, 100);
	key_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink, 0));

	//TODOMO : 이미지로하면 텍스트는 제거
	sound_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	sound_Button->screenTextRenderer->SetText(L"소리");
	sound_Button->screenTextRenderer->SetFontSize(30);
	sound_Button->screenTextRenderer->SetFontName(L"덕온공주옛체");

	key_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	key_Button->screenTextRenderer->SetText(L"조작");
	key_Button->screenTextRenderer->SetFontSize(30);
	key_Button->screenTextRenderer->SetFontName(L"덕온공주옛체");

}

void Options::Update()
{
}

void Options::Destroyed()
{
}
