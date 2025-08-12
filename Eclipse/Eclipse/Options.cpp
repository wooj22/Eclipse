#include "Options.h"
#include "SoundManager.h"
#include "../Direct2D_EngineLib/AudioSystem.h"

Options::Options() : GameObject("Optionts", "Optionts")
{
	rectTransform = AddComponent<RectTransform>();

	optionWindowBackGround_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	optionWindowBackGroundGradient_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	background_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	close_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	sound_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	key_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	soundBase_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	controlKeyBase_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	leftClick_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	rightClick_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	soundMaster_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	soundBGM_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	soundSFX_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	soundAMB_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	soundMaster_slider = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Slider>();
	soundBGM_slider = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Slider>();
	soundSFX_slider = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Slider>();
	soundAMB_slider = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Slider>();

	for (int i = 0; i < 10; ++i)
	{
		auto name = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
		name->rectTransform->SetParent(controlKeyBase_Image->rectTransform);
		controlKeyNames.push_back(name);
		controlKeyNames[i]->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
		controlKeyNames[i]->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Cyan));


		auto image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
		image->rectTransform->SetParent(controlKeyBase_Image->rectTransform);
		controlKeyImage.push_back(image);

		if (i < 8)
		{
			auto desc = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
			desc->rectTransform->SetParent(controlKeyBase_Image->rectTransform);
			controlKeyDescriptions.push_back(desc);
		}
		if (i < 2)
		{
			auto title = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
			title->rectTransform->SetParent(controlKeyBase_Image->rectTransform);
			controlKeyTitles.push_back(title);
		}
	}

	optionWindowBackGround_Image->rectTransform->SetParent(this->rectTransform);
	optionWindowBackGroundGradient_Image->rectTransform->SetParent(this->rectTransform);
	background_Image->rectTransform->SetParent(this->rectTransform);
	close_Button->rectTransform->SetParent(this->rectTransform);
	sound_Button->rectTransform->SetParent(this->rectTransform);
	key_Button->rectTransform->SetParent(this->rectTransform);
	soundBase_Image->rectTransform->SetParent(this->rectTransform);
	soundMaster_Text->rectTransform->SetParent(soundBase_Image->rectTransform);
	soundBGM_Text->rectTransform->SetParent(soundBase_Image->rectTransform);
	soundSFX_Text->rectTransform->SetParent(soundBase_Image->rectTransform);
	soundAMB_Text->rectTransform->SetParent(soundBase_Image->rectTransform);
	soundMaster_slider->rectTransform->SetParent(soundMaster_Text->rectTransform);
	soundBGM_slider->rectTransform->SetParent(soundBGM_Text->rectTransform);
	soundSFX_slider->rectTransform->SetParent(soundSFX_Text->rectTransform);
	soundAMB_slider->rectTransform->SetParent(soundAMB_Text->rectTransform);

	controlKeyBase_Image->rectTransform->SetParent(this->rectTransform);
	leftClick_Image->rectTransform->SetParent(controlKeyBase_Image->rectTransform);
	rightClick_Image->rectTransform->SetParent(controlKeyBase_Image->rectTransform);

	// 제목
	std::wstring titles[] = { L"이동\n상호작용", L"전투\n기술조작" };
	for (int i = 0; i < 2; i++)
	{
		controlKeyTitles[i]->rectTransform->SetParent(controlKeyBase_Image->rectTransform);
		controlKeyTitles[i]->screenTextRenderer->SetText(titles[i]);
		controlKeyTitles[i]->screenTextRenderer->SetFontSize(22);
	}

	// 이름
	std::wstring names[] = {
		L"좌/우 이동", L"낙하", L"도약", L"성장", L"상호작용",
		L"공격", L"공격 취소", L"돌진", L"흡수", L"방출"
	};
	for (int i = 0; i < 10; i++)
	{
		controlKeyNames[i]->screenTextRenderer->SetText(names[i]);
	}

	// 키 설명
	std::wstring desc[] = {
		L"A/D", L"S", L"SPACE BAR", L"TAB", L"F",
		L"SHIFT", L"Q", L"E"
	};
	for (int i = 0; i < 8; i++)
	{
		controlKeyDescriptions[i]->screenTextRenderer->SetText(desc[i]);
	}
}

void Options::Awake()
{

}

void Options::SceneStart()
{
	rectTransform->SetPosition(0, -150);
	optionWindowBackGround_Image->rectTransform->SetSize(1920, 1080);
	optionWindowBackGround_Image->rectTransform->SetPosition(0, 150);
	optionWindowBackGroundGradient_Image->rectTransform->SetSize(1920, 1080);
	optionWindowBackGroundGradient_Image->rectTransform->SetPosition(0, 150);
	background_Image->rectTransform->SetSize(850, 600);

	auto backImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/OptionsBackGround.png");
	background_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(backImageTexture, "OptionsBackGround");
	auto optionWindowBackGroundImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/WindowBackGround.png");
	optionWindowBackGround_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(optionWindowBackGroundImageTexture, "WindowBackGround");
	auto optionWindowBackGroundGradientImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/WindowBackGroundGradient.png");
	optionWindowBackGroundGradient_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(optionWindowBackGroundGradientImageTexture, "WindowBackGroundGradient");

	close_Button->rectTransform->SetPosition(350, 230);
	close_Button->rectTransform->SetSize(83, 79);
	auto closeButtonTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/BackButton.png");
	close_Button->imageRenderer->sprite = ResourceManager::Get().CreateSprite(closeButtonTexture, "BackButton");

	sound_Button->rectTransform->SetPosition(-330, 200);
	sound_Button->rectTransform->SetSize(200, 100);
	sound_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink, 0));

	key_Button->rectTransform->SetPosition(-330, 100);
	key_Button->rectTransform->SetSize(200, 100);
	key_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink, 0));

	sound_Button->screenTextRenderer->SetText(L"소리");
	sound_Button->screenTextRenderer->SetFontSize(60);
	sound_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Cyan));
	sound_Button->screenTextRenderer->SetFontName(L"덕온공주체");

	key_Button->screenTextRenderer->SetText(L"조작");
	key_Button->screenTextRenderer->SetFontSize(60);
	key_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Cyan));
	key_Button->screenTextRenderer->SetFontName(L"덕온공주체");

	soundBase_Image->rectTransform->SetPosition(52, 5);
	soundBase_Image->rectTransform->SetSize(588, 385); 
	auto optionSoundTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/OptionSound.png");
	soundBase_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(optionSoundTexture, "OptionSound");

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
	soundBGM_slider->rectTransform->SetPosition(0,-50);
	soundSFX_slider->rectTransform->SetPosition(0,-50);
	soundAMB_slider->rectTransform->SetPosition(0,-50);

	soundMaster_slider->SetGuageImageAlpha();	
	soundBGM_slider->SetGuageImageAlpha();
	soundSFX_slider->SetGuageImageAlpha();
	soundAMB_slider->SetGuageImageAlpha();

	controlKeyBase_Image->rectTransform->SetPosition(50, 0);
	controlKeyBase_Image->rectTransform->SetSize(600, 400);
	auto controlKetBaseTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/ControlKetBase.png");
	controlKeyBase_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(controlKetBaseTexture, "ControlKetBase");
	
	auto leftClick = ResourceManager::Get().CreateTexture2D("../Resource/mo/LeftClick.png");
	leftClick_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(leftClick, "LeftClick");
	leftClick_Image->rectTransform->SetPosition(100,-50);
	leftClick_Image->rectTransform->SetSize(30,30);

	auto rightClick = ResourceManager::Get().CreateTexture2D("../Resource/mo/RightClick.png");
	rightClick_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(rightClick, "RightClick");
	rightClick_Image->rectTransform->SetPosition(100, -85);
	rightClick_Image->rectTransform->SetSize(30, 30);

	auto controlKetImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/ControlKetImage.png");
	for (int i = 0; i < 10; i++)
	{
		controlKeyImage[i]->imageRenderer->sprite = ResourceManager::Get().CreateSprite(controlKetImageTexture, "ControlKetImage");
		controlKeyImage[i]->rectTransform->SetSize(350,50);
		auto y =  i > 4 ? i * 35 + 75 : i * 35;
		controlKeyImage[i]->rectTransform->SetPosition(50,200 - y);

		controlKeyNames[i]->rectTransform->SetSize(350, 50);
		controlKeyNames[i]->rectTransform->SetPosition(100, 200 - y);

		if (i < 8)
		{
			auto new_y = i > 4 ? y+70: y;
			controlKeyDescriptions[i]->rectTransform->SetSize(350, 50);
			controlKeyDescriptions[i]->rectTransform->SetPosition(100, 200 - new_y);
		}
	}

	// Options.cpp
	
	// [ 기존 코드 ]
	//controlKeyTitles[0]->rectTransform->SetPosition(-200, 125);
	//controlKeyTitles[1]->rectTransform->SetPosition(-200, -125);

	//soundMaster_slider->slider->SetValue(AudioSystem::Get().GetMasterVolume());
	//soundBGM_slider->slider->SetValue(AudioSystem::Get().GetBGMVolume());
	//soundSFX_slider->slider->SetValue(AudioSystem::Get().GetSFXVolume());
	//soundAMB_slider->slider->SetValue(AudioSystem::Get().GetAMBVolume());
	//
	//soundMaster_slider->slider->onValueChangedListeners.AddListener(this, [this]() { AudioSystem::Get().SetMasterVolume(soundMaster_slider->GetComponent<Slider>()->GetValue()); });
	//soundBGM_slider->slider->onValueChangedListeners.AddListener(this, [this]() { AudioSystem::Get().SetBGMVolume(soundBGM_slider->GetComponent<Slider>()->GetValue()); });
	//soundSFX_slider->slider->onValueChangedListeners.AddListener(this, [this]() { AudioSystem::Get().SetSFXVolume(soundSFX_slider->GetComponent<Slider>()->GetValue()); });
	//soundAMB_slider->slider->onValueChangedListeners.AddListener(this, [this]() { AudioSystem::Get().SetAMBVolume(soundAMB_slider->GetComponent<Slider>()->GetValue()); });

	// [ 바뀐 코드 ] 

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
}

void Options::Update()
{

}

void Options::Destroyed()
{
}

