#include "TitleUI.h"

void TitleUI::Awake()
{
}

void TitleUI::SceneStart()
{
	menuButtons = { play_Button, options_Button, credit_Button, end_Button };
	optionButtons = { optionUI->sound_Button,optionUI->key_Button };
	backgroundImage->rectTransform->SetPivot(0.5, 0.5);
	backgroundImage->rectTransform->SetSize(1920, 1080);
	auto title = ResourceManager::Get().CreateTexture2D("../Resource/mo/TitleBackGround.png");
	backgroundImage->imageRenderer->sprite = ResourceManager::Get().CreateSprite(title, "TitleBackGround");

	options_Button->rectTransform->SetParent(play_Button->rectTransform);
	credit_Button->rectTransform->SetParent(play_Button->rectTransform);
	end_Button->rectTransform->SetParent(play_Button->rectTransform);

	play_Button->rectTransform->SetPosition(0, 100);
	play_Button->rectTransform->SetSize(200, 100);
	auto playButtonTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/PlayButton.png");
	play_Button->imageRenderer->sprite = ResourceManager::Get().CreateSprite(playButtonTexture, "PlayButton");

	options_Button->rectTransform->SetPosition(0, -100);
	options_Button->rectTransform->SetSize(200, 100);
	options_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink,0));

	credit_Button->rectTransform->SetPosition(0, -200);
	credit_Button->rectTransform->SetSize(200, 100);
	credit_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink, 0));

	end_Button->rectTransform->SetPosition(0, -300);
	end_Button->rectTransform->SetSize(200, 100);
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

	// 옵션 UI 생성
	optionUI->SetActive(false);
	optionUI->rectTransform->SetPosition(0, 0);

	options_Button->button->onClickListeners.AddListener(
		this, std::bind(&TitleUI::OpenOptionUI, this));

	for (UI_Button* btn : menuButtons)
	{
		btn->button->onPointEnterListeners.AddListener(
			this, std::bind(&TitleUI::OnPointEnterButton, this, btn));

		btn->button->onPointExitListeners.AddListener(
			this, std::bind(&TitleUI::OnPointExitButton, this, btn));
	}

	for (UI_Button* btn : optionButtons)
	{
		btn->button->onClickListeners.AddListener(
			this, std::bind(&TitleUI::OnClickOptionUI, this,btn));
	}


	optionUI->close_Button->button->onClickListeners.AddListener(
		this, [this]() { play_Button->SetActive(true); });
}

void TitleUI::Update()
{

}

void TitleUI::Destroyed()
{

}

void TitleUI::OnPointEnterButton(UI_Button* onButton)
{
	prevHoverButton = onButton;
	onButton->imageRenderer->SetAlpha(1);
	underscore_Image->rectTransform->SetParent(onButton->rectTransform);
	underscore_Image->SetActive(true);
}

void TitleUI::OnPointExitButton(UI_Button* currButton)
{
	if (prevHoverButton == currButton)
		underscore_Image->SetActive(false);
}

void TitleUI::OnClickOptionUI(UI_Button* button)
{
	RectTransform* target = nullptr;

	if (button == optionUI->sound_Button)
		target = optionUI->sound_Button->rectTransform;
	else if (button == optionUI->key_Button)
		target = optionUI->key_Button->rectTransform;

	if (target && underscore_Image->rectTransform->GetParent() != target)
		underscore_Image->rectTransform->SetParent(target);
}

void TitleUI::OpenOptionUI()
{
	underscore_Image->rectTransform->SetParent(optionUI->sound_Button->rectTransform);
	underscore_Image->SetActive(true);
	optionUI->SetActive(true);
	play_Button->SetActive(false);
}
