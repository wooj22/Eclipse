#include "TitleUI.h"

void TitleUI::Awake()
{
}

void TitleUI::SceneStart()
{
	menuButtons = { play_Button, options_Button, credit_Button, end_Button };
	backgroundImage->rectTransform->SetPivot(0.5, 0.5);
	backgroundImage->rectTransform->SetSize(1920, 1080);
	auto title = ResourceManager::Get().CreateTexture2D("../Resource/mo/TitleBackGround.png");
	backgroundImage->imageRenderer->sprite = ResourceManager::Get().CreateSprite(title, "TitleBackGround");

	play_Button->rectTransform->SetPosition(0, 100);
	play_Button->rectTransform->SetSize(200, 90);
	auto playButtonTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/PlayButton.png");
	play_Button->imageRenderer->sprite = ResourceManager::Get().CreateSprite(playButtonTexture, "PlayButton");

	options_Button->rectTransform->SetPosition(0, 0);
	options_Button->rectTransform->SetSize(200, 90);
	options_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink,0));

	credit_Button->rectTransform->SetPosition(0, -100);
	credit_Button->rectTransform->SetSize(200, 90);
	credit_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink, 0));

	end_Button->rectTransform->SetPosition(0, -200);
	end_Button->rectTransform->SetSize(200, 90);
	end_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink,0));

	//TODOMO : 이미지로하면 텍스트는 제거
	options_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	options_Button->screenTextRenderer->SetText(L"옵션");
	options_Button->screenTextRenderer->SetFontSize(30);
	credit_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	credit_Button->screenTextRenderer->SetText(L"크레딧");
	credit_Button->screenTextRenderer->SetFontSize(30);
	end_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	end_Button->screenTextRenderer->SetText(L"게임 종료");
	end_Button->screenTextRenderer->SetFontSize(30);

	// 밑줄 이미지
	underscore_Image->rectTransform->SetPosition(0, -30); // TODOMO : 버튼 이미지 나오면 위치 수정
	underscore_Image->rectTransform->SetSize(200, 150);
	auto underScoreTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Underscore.png");
	underscore_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(underScoreTexture, "Underscore");
	underscore_Image->SetActive(false); // 초기에는 밑줄 이미지 비활성화

	/*play_Button->button->onClickListeners.AddListener(
		this, std::bind(&SceneManager::ChangeScene(), this));*/

	for (UI_Button* btn : menuButtons)
	{
		btn->button->onPointEnterListeners.AddListener(
			this,std::bind(&TitleUI::OnPointEnterButton, this, btn));
		btn->button->onPointExitListeners.AddListener(
			this, std::bind(&TitleUI::OnPointExitButton, this));
	}
}

void TitleUI::Update()
{

}

void TitleUI::Destroyed()
{

}

void TitleUI::OnPointEnterButton(UI_Button* parent)
{
	underscore_Image->SetActive(true);
	underscore_Image->rectTransform->SetParent(parent->rectTransform);
}

void TitleUI::OnPointExitButton()
{
	underscore_Image->SetActive(false);
}

