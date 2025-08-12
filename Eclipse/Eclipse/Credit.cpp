#include "Credit.h"

void Credit::Awake()
{
	creditWindowBackGround_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	creditWindowBackGroundGradient_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	background_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	splitter_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	close_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	creditTitle_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	plannerTitle_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	artTitle_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	programmerTitle_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	plannerName_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	artName_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	programmerName_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();

	creditWindowBackGroundGradient_Image->rectTransform->SetParent(creditWindowBackGround_Image->rectTransform);
	background_Image->rectTransform->SetParent(creditWindowBackGround_Image->rectTransform);
	splitter_Image->rectTransform->SetParent(background_Image->rectTransform);
	close_Button->rectTransform->SetParent(background_Image->rectTransform);
	creditTitle_Text->rectTransform->SetParent(background_Image->rectTransform);
	plannerTitle_Text->rectTransform->SetParent(background_Image->rectTransform);
	artTitle_Text->rectTransform->SetParent(background_Image->rectTransform);
	programmerTitle_Text->rectTransform->SetParent(background_Image->rectTransform);
	plannerName_Text->rectTransform->SetParent(background_Image->rectTransform);
	artName_Text->rectTransform->SetParent(background_Image->rectTransform);
	programmerName_Text->rectTransform->SetParent(background_Image->rectTransform);

	creditWindowBackGround_Image->SetActive(false);
}

void Credit::SceneStart()
{
	creditWindowBackGround_Image->rectTransform->SetSize(1920, 1080);
	creditWindowBackGroundGradient_Image->rectTransform->SetSize(1920, 1080);
	background_Image->rectTransform->SetSize(850, 600);
	background_Image->rectTransform->SetPosition(0, -150);
	splitter_Image->rectTransform->SetSize(63, 591);

	auto WindowBackGroundImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/WindowBackGround.png");
	creditWindowBackGround_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(WindowBackGroundImageTexture, "WindowBackGround");
	auto WindowBackGroundGradientImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/WindowBackGroundGradient.png");
	creditWindowBackGroundGradient_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(WindowBackGroundGradientImageTexture, "WindowBackGroundGradient");
	auto backImagenTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/OptionsBackGround.png");
	background_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(backImagenTexture, "OptionsBackGround");
	auto creditSplitterImage = ResourceManager::Get().CreateTexture2D("../Resource/mo/CreditSplitterImage.png");
	splitter_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(creditSplitterImage, "CreditSplitterImage");

	close_Button->rectTransform->SetPosition(350, 230);
	close_Button->rectTransform->SetSize(83, 79);
	auto closeButtonTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/BackButton.png");
	close_Button->imageRenderer->sprite = ResourceManager::Get().CreateSprite(closeButtonTexture, "BackButton");


	splitter_Image->rectTransform->SetPosition(-200, 0);

	creditTitle_Text->screenTextRenderer->SetText(L"Å©·¹µ÷");
	creditTitle_Text->screenTextRenderer->SetFontName(L"´ö¿Â°øÁÖÃ¼");
	creditTitle_Text->screenTextRenderer->SetFontSize(60);
	creditTitle_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Cyan));
	creditTitle_Text->rectTransform->SetPosition(-300,200);
	creditTitle_Text->rectTransform->SetSize(150,60);

	plannerTitle_Text->screenTextRenderer->SetText(L"±âÈ¹");
	plannerTitle_Text->screenTextRenderer->SetFontName(L"´ö¿Â°øÁÖÃ¼");
	plannerTitle_Text->screenTextRenderer->SetFontSize(30);
	plannerTitle_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Cyan));
	plannerTitle_Text->rectTransform->SetPosition(-100, 150);
	plannerTitle_Text->rectTransform->SetSize(150, 60);

	plannerName_Text->screenTextRenderer->SetText(L"À±µ¿ÁØ\t   Á¶ÇöÁ¤");
	plannerName_Text->screenTextRenderer->SetFontSize(30);
	plannerName_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	plannerName_Text->rectTransform->SetPosition(100, 150);
	plannerName_Text->rectTransform->SetSize(350, 60);


	artTitle_Text->screenTextRenderer->SetText(L"¾ÆÆ®");
	artTitle_Text->screenTextRenderer->SetFontName(L"´ö¿Â°øÁÖÃ¼");
	artTitle_Text->screenTextRenderer->SetFontSize(30);
	artTitle_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Cyan));
	artTitle_Text->rectTransform->SetPosition(-100, 50);
	artTitle_Text->rectTransform->SetSize(150, 60);

	artName_Text->screenTextRenderer->SetText(L"¼Õ¹Î°æ\t   °­Çýºó");
	artName_Text->screenTextRenderer->SetFontSize(30);
	artName_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	artName_Text->rectTransform->SetPosition(100, 50);
	artName_Text->rectTransform->SetSize(350, 60);

	programmerTitle_Text->screenTextRenderer->SetText(L"°³¹ß");
	programmerTitle_Text->screenTextRenderer->SetFontName(L"´ö¿Â°øÁÖÃ¼");
	programmerTitle_Text->screenTextRenderer->SetFontSize(30);
	programmerTitle_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Cyan));
	programmerTitle_Text->rectTransform->SetPosition(-100, -50);
	programmerTitle_Text->rectTransform->SetSize(150, 60);

	programmerName_Text->screenTextRenderer->SetText(L"Àå¾Æ·Ð\t   ¹®¼±¹Î\n\n¾ç¿ìÁ¤\t   ÇÑÀ±¸ð");
	programmerName_Text->screenTextRenderer->SetFontSize(30);
	programmerName_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	programmerName_Text->rectTransform->SetPosition(100, -90);
	programmerName_Text->rectTransform->SetSize(350, 60);

}

void Credit::Update()
{
}

void Credit::Destroyed()
{
}
