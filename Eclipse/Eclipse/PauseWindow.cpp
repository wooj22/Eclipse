#include "PauseWindow.h"
#include "EclipseApp.h"
#include "../Direct2D_EngineLib/GameApp.h"
#include "PlayUI.h"

void PauseWindow::Awake()
{
	rectTransform = AddComponent<RectTransform>();
	optionWindowBackGround_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	optionWindowBackGroundGradient_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	window_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	close_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	newGame_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	continuGame_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	quitGame_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	underscore_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();

	optionWindowBackGround_Image->rectTransform->SetParent(rectTransform);
	optionWindowBackGroundGradient_Image->rectTransform->SetParent(rectTransform);
	window_Image->rectTransform->SetParent(optionWindowBackGround_Image->rectTransform);
	close_Button->rectTransform->SetParent(optionWindowBackGround_Image->rectTransform);

	continuGame_Button->rectTransform->SetParent(optionWindowBackGround_Image->rectTransform);
	newGame_Button->rectTransform->SetParent(continuGame_Button->rectTransform);
	quitGame_Button->rectTransform->SetParent(continuGame_Button->rectTransform);
}

void PauseWindow::SceneStart()
{
	eventButtons = { continuGame_Button,newGame_Button , quitGame_Button };
	window_Image->rectTransform->SetSize(850, 600);
	optionWindowBackGround_Image->rectTransform->SetSize(1920, 1080);
	optionWindowBackGroundGradient_Image->rectTransform->SetSize(1920, 1080);

	auto windowImagenTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/OptionsBackGround.png");
	window_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(windowImagenTexture, "OptionsBackGround");
	auto optionWindowBackGroundImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/WindowBackGround.png");
	optionWindowBackGround_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(optionWindowBackGroundImageTexture, "WindowBackGround");
	auto optionWindowBackGroundGradientImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/WindowBackGroundGradient.png");
	optionWindowBackGroundGradient_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(optionWindowBackGroundGradientImageTexture, "WindowBackGroundGradient");


	close_Button->rectTransform->SetPosition(350, 230);
	close_Button->rectTransform->SetSize(83, 79);
	auto closeButtonTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/BackButton.png");
	close_Button->imageRenderer->sprite = ResourceManager::Get().CreateSprite(closeButtonTexture, "BackButton");

	continuGame_Button->rectTransform->SetPosition(0, -150);
	continuGame_Button->rectTransform->SetSize(200, 100);
	continuGame_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink, 0));
	
	newGame_Button->rectTransform->SetPosition(-200, 0);
	newGame_Button->rectTransform->SetSize(200, 100);
	newGame_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink, 0));
	
	quitGame_Button->rectTransform->SetPosition(200, 0);
	quitGame_Button->rectTransform->SetSize(200, 100);
	quitGame_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink, 0));

	//TODOMO : 이미지로하면 텍스트는 제거
	continuGame_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	continuGame_Button->screenTextRenderer->SetText(L"이어하기");
	continuGame_Button->screenTextRenderer->SetFontSize(30);
	continuGame_Button->screenTextRenderer->SetFontName(L"덕온공주체");
	newGame_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	newGame_Button->screenTextRenderer->SetText(L"처음으로");
	newGame_Button->screenTextRenderer->SetFontSize(30);
	newGame_Button->screenTextRenderer->SetFontName(L"덕온공주체");
	quitGame_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	quitGame_Button->screenTextRenderer->SetText(L"게임종료");
	quitGame_Button->screenTextRenderer->SetFontSize(30);
	quitGame_Button->screenTextRenderer->SetFontName(L"덕온공주체");

	// 밑줄 이미지
	underscore_Image->rectTransform->SetPosition(0, -30); // TODOMO : 버튼 이미지 나오면 위치 수정
	underscore_Image->rectTransform->SetSize(200, 150);
	auto underScoreTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Underscore.png");
	underscore_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(underScoreTexture, "Underscore");
	underscore_Image->SetActive(false); // 초기에는 밑줄 이미지 비활성화

	for (UI_Button* btn : eventButtons)
	{
		btn->button->onPointEnterListeners.AddListener(
			this, std::bind(&PauseWindow::OnPointEnterButton, this, btn));

		btn->button->onPointExitListeners.AddListener(
			this, std::bind(&PauseWindow::OnPointExitButton, this, btn));
	}

	newGame_Button->button->onClickListeners.AddListener(
		this, [this]() { 
			SceneManager::Get().ChangeScene(EclipseApp::MENU);
			GameManager::Get().g_playUI->ButtonClickSound(); });

	quitGame_Button->button->onClickListeners.AddListener(
		this, []() {
			GameManager::Get().g_playUI->ButtonClickSound(); 
			GameApp::Quit(); });

	SetActive(false);

}

void PauseWindow::Update()
{
}

void PauseWindow::OnPointEnterButton(UI_Button* onButton)
{
	prevHoverButton = onButton;
	onButton->imageRenderer->SetAlpha(1);
	underscore_Image->rectTransform->SetParent(onButton->rectTransform);
	underscore_Image->SetActive(true);
	GameManager::Get().g_playUI->ButtonEnterSound();
}

void PauseWindow::OnPointExitButton(UI_Button* currButton)
{
	if (prevHoverButton == currButton)
		underscore_Image->SetActive(false);
}