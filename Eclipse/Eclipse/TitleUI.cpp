#include "TitleUI.h"
#include "../Direct2D_EngineLib/GameApp.h"
#include "EclipseApp.h"
#include <DirectXMath.h>

void TitleUI::Awake()
{
	// audio source 컴포넌트 생성
	bgmSource = AddComponent<AudioSource>();
	sfxSource = AddComponent<AudioSource>();

	// audio clip 리소스 생성
	bgmClip = ResourceManager::Get().CreateAudioClip("../Resource/Audio/UI/BGM/s_Title.wav");
	sfxClip_Button1 = ResourceManager::Get().CreateAudioClip("../Resource/Audio/UI/SFX/Button/s_Button_1.wav");
	sfxClip_Button2 = ResourceManager::Get().CreateAudioClip("../Resource/Audio/UI/SFX/Button/s_Button_2.wav");

	// audioSource 채널 그룹 지정 및 사운드 재생
	bgmSource->SetChannelGroup(AudioSystem::Get().GetBGMGroup());
	bgmSource->SetVolume(0);
	bgmSource->SetClip(bgmClip);
	bgmSource->SetLoop(true);
	bgmSource->Play();

	sfxSource->SetChannelGroup(AudioSystem::Get().GetSFXGroup());
	sfxSource->SetClip(sfxClip_Button1);
	sfxSource->SetLoop(false);
}

void TitleUI::SceneStart()
{
	menuButtons = { play_Button, options_Button, credit_Button, end_Button };
	optionButtons = { optionUI->sound_Button,optionUI->key_Button };
	backgroundImage->AddComponent<Transform>();
	backgroundImage->AddComponent<SpriteRenderer>();
	titleLogo_Image->AddComponent<Transform>();
	titleLogo_Image->AddComponent<SpriteRenderer>();
	titleSpell_Image->AddComponent<Transform>();
	titleSpell_Image->AddComponent<SpriteRenderer>();
	auto title = ResourceManager::Get().CreateTexture2D("../Resource/mo/TitleBackGround.png");
	backgroundImage->GetComponent<SpriteRenderer>()->sprite = ResourceManager::Get().CreateSprite(title, "TitleBackGround");

	auto logo = ResourceManager::Get().CreateTexture2D("../Resource/mo/Title_Logo.png");
	titleLogo_Image->GetComponent<SpriteRenderer>()->sprite = ResourceManager::Get().CreateSprite(logo, "Title_Logo");
	titleLogo_Image->GetComponent<SpriteRenderer>()->renderMode = RenderMode::Lit_Glow;;

	auto spell = ResourceManager::Get().CreateTexture2D("../Resource/mo/Title_Spell.png");
	titleSpell_Image->GetComponent<SpriteRenderer>()->sprite = ResourceManager::Get().CreateSprite(spell, "Title_Spell");
	//titleSpell_Image->GetComponent<SpriteRenderer>()->renderMode = RenderMode::Lit_Glow;

	options_Button->rectTransform->SetParent(play_Button->rectTransform);
	credit_Button->rectTransform->SetParent(play_Button->rectTransform);
	end_Button->rectTransform->SetParent(play_Button->rectTransform);

	play_Button->rectTransform->SetPosition(0, -200);
	play_Button->rectTransform->SetSize(300, 50);
	play_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink, 0));

	options_Button->rectTransform->SetPosition(0, -50);
	options_Button->rectTransform->SetSize(300, 50);
	options_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink,0));

	credit_Button->rectTransform->SetPosition(0, -100);
	credit_Button->rectTransform->SetSize(300, 50);
	credit_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink, 0));

	end_Button->rectTransform->SetPosition(0, -150);
	end_Button->rectTransform->SetSize(300, 50);
	end_Button->imageRenderer->SetBaseColor(D2D1::ColorF(D2D1::ColorF::LightPink,0));

	//TODOMO : 이미지로하면 텍스트는 제거
	play_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	play_Button->screenTextRenderer->SetText(L"게임 시작");
	play_Button->screenTextRenderer->SetFontSize(30);
	play_Button->screenTextRenderer->SetFontName(L"덕온공주체");
	options_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	options_Button->screenTextRenderer->SetText(L"옵션");
	options_Button->screenTextRenderer->SetFontSize(30);
	options_Button->screenTextRenderer->SetFontName(L"덕온공주체");
	credit_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	credit_Button->screenTextRenderer->SetText(L"크레딧");
	credit_Button->screenTextRenderer->SetFontSize(30);
	credit_Button->screenTextRenderer->SetFontName(L"덕온공주체");
	end_Button->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	end_Button->screenTextRenderer->SetText(L"게임 종료");
	end_Button->screenTextRenderer->SetFontSize(30);
	end_Button->screenTextRenderer->SetFontName(L"덕온공주체");

	// 밑줄 이미지
	underscore_Image->rectTransform->SetPosition(0, -30); // TODOMO : 버튼 이미지 나오면 위치 수정
	underscore_Image->rectTransform->SetSize(150, 150);
	auto underScoreTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Underscore.png");
	underscore_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(underScoreTexture, "Underscore");
	underscore_Image->SetActive(false); // 초기에는 밑줄 이미지 비활성화

	// 옵션 UI 생성
	optionUI->SetActive(false);
	optionUI->rectTransform->SetPosition(0, 0);

	play_Button->button->onClickListeners.AddListener(
		this, std::bind(&TitleUI::ChangePlayScene, this));

	options_Button->button->onClickListeners.AddListener(
		this, std::bind(&TitleUI::OpenOptionUI, this));

	optionUI->close_Button->button->onClickListeners.AddListener(
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
		this, [this]() {
			sfxSource->SetClip(sfxClip_Button2);
			sfxSource->Play(); 
			play_Button->SetActive(true);
			optionUI->SetActive(false);
		});
	/*optionUI->close_Button->button->onClickListeners.AddListener(
		this, [this]() {underscore_Image->rectTransform->SetSize(150, 150); });*/

	end_Button->button->onClickListeners.AddListener(
		this, [this]() {
			sfxSource->SetClip(sfxClip_Button2);
			sfxSource->Play(); 
			GameApp::Quit(); });
}

void TitleUI::Update()
{
	titleSpell_Image->transform->Rotate(0.05);

	glowtimer += Time::GetDeltaTime();  // 매 프레임 시간 누적

	//float glow = ((sinf(glowtimer * glowspeed - DirectX::XM_PIDIV2) + 1.0f) / 2.0f) * 1000.0f;
	float s = sinf(glowtimer * glowspeed) * 0.5f + 0.5f;
	s = pow(s, 6.0f);
	float glow = s * 150.0f;
	titleLogo_Image->GetComponent<SpriteRenderer>()->SetGlowAmmount(glow);

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

	sfxSource->SetClip(sfxClip_Button1);
	sfxSource->Play();
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

	sfxSource->SetClip(sfxClip_Button2);
	sfxSource->Play();
}

void TitleUI::OpenOptionUI()
{
	underscore_Image->rectTransform->SetParent(optionUI->sound_Button->rectTransform);
	underscore_Image->SetActive(true);
	//underscore_Image->rectTransform->SetSize(130, 150);
	optionUI->SetActive(true);
	play_Button->SetActive(false);
	sfxSource->SetClip(sfxClip_Button2);
	sfxSource->Play();
}


void TitleUI::ChangePlayScene()
{
	sfxSource->SetClip(sfxClip_Button2);
	sfxSource->Play();
	SceneManager::Get().ChangeScene(EclipseApp::PLAY);
}
