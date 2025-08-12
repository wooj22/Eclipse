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
	soundBase_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	soundMaster_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	soundBGM_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	soundSFX_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	soundAMB_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	soundMaster_slider = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Slider>();
	soundBGM_slider = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Slider>();
	soundSFX_slider = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Slider>();
	soundAMB_slider = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Slider>();

	optionWindowBackGround_Image->rectTransform->SetParent(rectTransform);
	optionWindowBackGroundGradient_Image->rectTransform->SetParent(rectTransform);
	window_Image->rectTransform->SetParent(optionWindowBackGround_Image->rectTransform);
	close_Button->rectTransform->SetParent(optionWindowBackGround_Image->rectTransform);

	continuGame_Button->rectTransform->SetParent(optionWindowBackGround_Image->rectTransform);
	newGame_Button->rectTransform->SetParent(optionWindowBackGround_Image->rectTransform);
	quitGame_Button->rectTransform->SetParent(optionWindowBackGround_Image->rectTransform);

	soundBase_Image->rectTransform->SetParent(optionWindowBackGround_Image->rectTransform);
	soundMaster_Text->rectTransform->SetParent(window_Image->rectTransform);
	soundBGM_Text->rectTransform->SetParent(window_Image->rectTransform);
	soundSFX_Text->rectTransform->SetParent(window_Image->rectTransform);
	soundAMB_Text->rectTransform->SetParent(window_Image->rectTransform);
	soundMaster_slider->rectTransform->SetParent(soundMaster_Text->rectTransform);
	soundBGM_slider->rectTransform->SetParent(soundBGM_Text->rectTransform);
	soundSFX_slider->rectTransform->SetParent(soundSFX_Text->rectTransform);
	soundAMB_slider->rectTransform->SetParent(soundAMB_Text->rectTransform);

	linecontroller = new LineController();
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

	continuGame_Button->rectTransform->SetPosition(0, -225);
	continuGame_Button->rectTransform->SetSize(200, 30);
	continuGame_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink, 0));
	
	newGame_Button->rectTransform->SetPosition(-200, -225);
	newGame_Button->rectTransform->SetSize(200, 30);
	newGame_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink, 0));
	
	quitGame_Button->rectTransform->SetPosition(200, -225);
	quitGame_Button->rectTransform->SetSize(200, 30);
	quitGame_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink, 0));

	continuGame_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Cyan));
	continuGame_Button->screenTextRenderer->SetText(L"이어하기");
	continuGame_Button->screenTextRenderer->SetFontSize(30);
	continuGame_Button->screenTextRenderer->SetFontName(L"덕온공주체");
	newGame_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Cyan));
	newGame_Button->screenTextRenderer->SetText(L"처음으로");
	newGame_Button->screenTextRenderer->SetFontSize(30);
	newGame_Button->screenTextRenderer->SetFontName(L"덕온공주체");
	quitGame_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Cyan));
	quitGame_Button->screenTextRenderer->SetText(L"게임종료");
	quitGame_Button->screenTextRenderer->SetFontSize(30);
	quitGame_Button->screenTextRenderer->SetFontName(L"덕온공주체");

	soundBase_Image->rectTransform->SetPosition(0, 100);
	soundBase_Image->rectTransform->SetSize(700, 55);
	auto pauseSoundTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/PauseSound.png");
	soundBase_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(pauseSoundTexture, "PauseSound");

	soundMaster_Text->rectTransform->SetPosition(0, 200);
	soundMaster_Text->rectTransform->SetSize(550, 50);
	soundMaster_Text->screenTextRenderer->SetText(L"전체음량");

	soundBGM_Text->rectTransform->SetPosition(0, 75);
	soundBGM_Text->rectTransform->SetSize(550, 50);
	soundBGM_Text->screenTextRenderer->SetText(L"배경음");

	soundAMB_Text->rectTransform->SetPosition(0, -25);
	soundAMB_Text->rectTransform->SetSize(550, 50);
	soundAMB_Text->screenTextRenderer->SetText(L"환경음");

	soundSFX_Text->rectTransform->SetPosition(0, -125);
	soundSFX_Text->rectTransform->SetSize(550, 50);
	soundSFX_Text->screenTextRenderer->SetText(L"효과음");

	auto optionSoundSingle = ResourceManager::Get().CreateTexture2D("../Resource/mo/OptionSoundSingle.png");
	auto soundbar = ResourceManager::Get().CreateSprite(optionSoundSingle, "OptionSoundSingle");

	auto soundHandle = ResourceManager::Get().CreateTexture2D("../Resource/mo/SoundHandle.png");
	auto handle = ResourceManager::Get().CreateSprite(soundHandle, "SoundHandle");

	soundMaster_slider->SetSliderSize(450, 100);
	soundBGM_slider->SetSliderSize(450, 100);
	soundSFX_slider->SetSliderSize(450, 100);
	soundAMB_slider->SetSliderSize(450, 100);

	soundMaster_slider->SetHandleSize(100);
	soundBGM_slider->SetHandleSize(100);
	soundSFX_slider->SetHandleSize(100);
	soundAMB_slider->SetHandleSize(100);

	soundMaster_slider->SetBackgroundImage(soundbar);
	soundBGM_slider->SetBackgroundImage(soundbar);
	soundSFX_slider->SetBackgroundImage(soundbar);
	soundAMB_slider->SetBackgroundImage(soundbar);

	soundMaster_slider->SetHandleImage(handle);
	soundBGM_slider->SetHandleImage(handle);
	soundSFX_slider->SetHandleImage(handle);
	soundAMB_slider->SetHandleImage(handle);

	soundMaster_slider->rectTransform->SetPosition(0, -50);
	soundBGM_slider->rectTransform->SetPosition(0, -50);
	soundSFX_slider->rectTransform->SetPosition(0, -50);
	soundAMB_slider->rectTransform->SetPosition(0, -50);

	soundMaster_slider->SetGuageImageAlpha();
	soundBGM_slider->SetGuageImageAlpha();
	soundSFX_slider->SetGuageImageAlpha();
	soundAMB_slider->SetGuageImageAlpha();

	// 밑줄 이미지
	underscore_Image->rectTransform->SetPosition(0, -20);
	underscore_Image->rectTransform->SetSize(200, 150);
	auto underscoreTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Underscore.png");
	underscore_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(underscoreTexture, "Underscore");
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

	//soundMaster_slider->slider->SetValue(AudioSystem::Get().GetMasterVolume());
	//soundBGM_slider->slider->SetValue(AudioSystem::Get().GetBGMVolume());
	//soundSFX_slider->slider->SetValue(AudioSystem::Get().GetSFXVolume());
	//soundAMB_slider->slider->SetValue(AudioSystem::Get().GetAMBVolume());

	//soundMaster_slider->slider->onValueChangedListeners.AddListener(this, [this]() { AudioSystem::Get().SetMasterVolume(soundMaster_slider->GetComponent<Slider>()->GetValue()); });
	//soundBGM_slider->slider->onValueChangedListeners.AddListener(this, [this]() { AudioSystem::Get().SetBGMVolume(soundBGM_slider->GetComponent<Slider>()->GetValue()); });
	//soundSFX_slider->slider->onValueChangedListeners.AddListener(this, [this]() { AudioSystem::Get().SetSFXVolume(soundSFX_slider->GetComponent<Slider>()->GetValue()); });
	//soundAMB_slider->slider->onValueChangedListeners.AddListener(this, [this]() { AudioSystem::Get().SetAMBVolume(soundAMB_slider->GetComponent<Slider>()->GetValue()); });

	// [ 변경된 코드 ] 

	// 1. BGM 슬라이더
	soundBGM_slider->slider->SetValue(SoundManager::Get().GetBGMVolume()); // 옵션 볼륨 기준 값
	soundBGM_slider->slider->onValueChangedListeners.AddListener(this,
		[this]() { SoundManager::Get().SetBGMVolume(soundBGM_slider->GetComponent<Slider>()->GetValue()); });

	// 2. Master 슬라이더
	soundMaster_slider->slider->SetValue(SoundManager::Get().GetMasterVolume());
	soundMaster_slider->slider->onValueChangedListeners.AddListener(this,
		[this]() { SoundManager::Get().SetMasterVolume(soundMaster_slider->GetComponent<Slider>()->GetValue()); });

	// 3. SFX 슬라이더
	soundSFX_slider->slider->SetValue(SoundManager::Get().GetSFXVolume());
	soundSFX_slider->slider->onValueChangedListeners.AddListener(this,
		[this]() { SoundManager::Get().SetSFXVolume(soundSFX_slider->GetComponent<Slider>()->GetValue()); });

	// 4. AMB 슬라이더
	soundAMB_slider->slider->SetValue(SoundManager::Get().GetAMBVolume());
	soundAMB_slider->slider->onValueChangedListeners.AddListener(this,
		[this]() { SoundManager::Get().SetAMBVolume(soundAMB_slider->GetComponent<Slider>()->GetValue()); });

	SetActive(false);
}

void PauseWindow::Update()
{
}

void PauseWindow::Destroyed()
{
	delete linecontroller;
}

void PauseWindow::OnPointEnterButton(UI_Button* onButton)
{
	prevHoverButton = onButton;
	onButton->imageRenderer->SetAlpha(1);
	underscore_Image->SetActive(true);
	underscore_Image->rectTransform->SetParent(onButton->rectTransform);
	linecontroller->PlayAnimation("UI_Line");
	bool tr = underscore_Image->IsActive();
	bool tr2 = underscore_Image->rectTransform->GetParent()->gameObject->IsActive();
	GameManager::Get().g_playUI->ButtonEnterSound();
}

void PauseWindow::OnPointExitButton(UI_Button* currButton)
{
	if (prevHoverButton == currButton)
		underscore_Image->SetActive(false);
}