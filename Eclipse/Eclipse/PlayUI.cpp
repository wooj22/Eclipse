#define NOMINMAX
#include "PlayUI.h"
#include "Chat.h"
#include "Quest.h"
#include "NPC.h"
#include "EclipseApp.h"
#include "../Direct2D_EngineLib/Input.h"
#include <algorithm>

void PlayUI::Awake()
{  
	//해당 씬에 게임 오브젝트 생성
	timer_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	stop_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	pauseWindow = SceneManager::Get().GetCurrentScene()->CreateObject<PauseWindow>();
	quest_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	questName_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	quest_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	questCount_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	chat_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	chat_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	chat_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	hon_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	hon_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	skill1Icon_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	skill1_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	skill1CollTimeFilter_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	skill1Key_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	skill2Icon_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	skill2_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	skill2Key_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	waveInfo_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	skillWindowClose_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	skillWindowBackGround_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	skillWindowBackGroundGradient_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	skillWindow_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	skillWindowSplitter_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	skillWindowName_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::MoveSpeedUp));
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::AttackRangeUp));
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::KnockbackDistanceUp));
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::DoubleJump));
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::WallJump));
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::JumpAttackExtra));
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::Dash));
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::SkillCooldownDown));
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::FastFall));
	skillHonBig_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	skillHon_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	skillHon_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	bossHP = SceneManager::Get().GetCurrentScene()->CreateObject<BossHP>();
	npc = SceneManager::Get().GetCurrentScene()->CreateObject<NPC>({ 0,-800 });
	tooltip1 = SceneManager::Get().GetCurrentScene()->CreateObject<tooltip>();
	tooltip2 = SceneManager::Get().GetCurrentScene()->CreateObject<tooltip>();

	// audio source 컴포넌트 생성
	bgmSource = AddComponent<AudioSource>();
	sfxSource = AddComponent<AudioSource>();

	// audio clip 리소스 생성
	bgmClip_Main = ResourceManager::Get().CreateAudioClip("../Resource/Audio/UI/BGM/s_Main.wav");
	bgmClip_Wave = ResourceManager::Get().CreateAudioClip("../Resource/Audio/UI/BGM/s_Battle.wav");
	bgmClip_Boss = ResourceManager::Get().CreateAudioClip("../Resource/Audio/UI/BGM/s_Boss.wav");
	//bgmClip_Clear = ResourceManager::Get().CreateAudioClip("../Resource/Audio/UI/BGM/s_Clear.wav");
	
	sfxClip_Button1 = ResourceManager::Get().CreateAudioClip("../Resource/Audio/UI/SFX/Button/s_Button_1.wav");
	sfxClip_Button2 = ResourceManager::Get().CreateAudioClip("../Resource/Audio/UI/SFX/Button/s_Button_2.wav");
	sfxClip_SkillUI = ResourceManager::Get().CreateAudioClip("../Resource/Audio/UI/SFX/SkillUI/s_SkillUI.wav");
	sfxClip_SkillActive = ResourceManager::Get().CreateAudioClip("../Resource/Audio/UI/SFX/SKillUI/s_Skillactive.wav");
	//sfxClip_GameOver = ResourceManager::Get().CreateAudioClip("../Resource/Audio/UI/SFX/Scene/s_Defeat.wav");
	//sfxClip_ChangeScene = ResourceManager::Get().CreateAudioClip("../Resource/Audio/UI/SFX/Scene/s_Fadeinout.wav");
	

	// audioSource 채널 그룹 지정 및 사운드 재생
	bgmSource->SetChannelGroup(AudioSystem::Get().GetBGMGroup());
	sfxSource->SetChannelGroup(AudioSystem::Get().GetSFXGroup());
	//bgmSource->SetVolume(1);
	bgmSource->SetLoop(true);
	bgmSource->SetClip(bgmClip_Main);
	bgmSource->Play();

	sfxSource->SetLoop(false);
	sfxSource->SetClip(sfxClip_Button1);
}

void PlayUI::SceneStart()
{
	pauseCheckButtos = { stop_Button, pauseWindow->close_Button, pauseWindow->continuGame_Button };

	// 웨이브 타이머 UI
	timer_Text->rectTransform->SetPosition(0, 400);
	timer_Text->rectTransform->SetSize(500, 40);
	timer_Text->screenTextRenderer->SetFontSize(140);
	timer_Text->screenTextRenderer->SetFontName(L"덕온공주체");

	//waveInfo_Text->rectTransform->SetPosition(0, 400);
	waveInfo_Text->rectTransform->SetSize(1920, 40);
	waveInfo_Text->screenTextRenderer->SetFontSize(200);
	waveInfo_Text->SetActive(false);
	waveInfo_Text->screenTextRenderer->SetFontName(L"덕온공주체");

	// 일시 정지 버튼
	stop_Button->rectTransform->SetPosition(920, 500);
	stop_Button->rectTransform->SetSize(60, 60);
	auto stopButtonTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/StopButton.png");
	stop_Button->imageRenderer->sprite = ResourceManager::Get().CreateSprite(stopButtonTexture, "StopButton");

	
	// 퀘스트 창 UI
	questName_Text->rectTransform->SetParent(quest_Image->rectTransform);
	quest_Text->rectTransform->SetParent(quest_Image->rectTransform);
	questCount_Text->rectTransform->SetParent(quest_Image->rectTransform);
	quest_Image->rectTransform->SetPosition(850, 0);
	quest_Image->rectTransform->SetSize(250, 300);
	auto questImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Quest.png");
	quest_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(questImageTexture, "Quest");
	questName_Text->rectTransform->SetSize(300, 50);
	questName_Text->rectTransform->SetPosition(0, 50);
	questName_Text->screenTextRenderer->SetText(L"목표");
	questName_Text->screenTextRenderer->SetFontSize(30);
	questName_Text->screenTextRenderer->SetFontName(L"덕온공주체");
	quest_Text->rectTransform->SetSize(200, 50);
	/*quest_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
	quest_Text->screenTextRenderer->SetVerticalAlign(TextVerticalAlign::Top);*/
	questCount_Text->rectTransform->SetPosition(0, -50);
	questCount_Text->rectTransform->SetSize(200, 0);
	/*questCount_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
	questCount_Text->screenTextRenderer->SetVerticalAlign(TextVerticalAlign::Top);*/
	quest = quest_Image->AddComponent<Quest>();


	// 대화창 UI
	chat_Text->rectTransform->SetParent(chat_Image->rectTransform);
	chat_Button->rectTransform->SetParent(chat_Image->rectTransform);
	chat_Button->button->onClickListeners.AddListener(
		this, std::bind(&PlayUI::ClickChatButton, this));

	chat_Image->SetActive(false);
	chat_Image->imageRenderer->layer = 1;
	chat_Text->screenTextRenderer->layer = 2;
	chat_Button->imageRenderer->layer = 3;
	chat_Image->rectTransform->SetPosition(0, -400);
	chat_Image->rectTransform->SetSize(800, 160);
	auto chatImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Chat.png");
	chat_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(chatImageTexture, "Chat");
	
	chat_Text->rectTransform->SetSize(400, 50);
	chat_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
	chat_Text->screenTextRenderer->SetVerticalAlign(TextVerticalAlign::Top);

	chat = chat_Text->AddComponent<Chat>();
	chat_Button->rectTransform->SetPosition(300, -40);
	chat_Button->rectTransform->SetSize(100, 50);
	auto chatButtonTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/ChatButton.png");
	chat_Button->imageRenderer->sprite = ResourceManager::Get().CreateSprite(chatButtonTexture, "ChatButton");


	// 혼 & 스킬 UI
	hon_Text->rectTransform->SetParent(hon_Image->rectTransform);
	skill1_Image->rectTransform->SetParent(hon_Image->rectTransform);
	skill2_Image->rectTransform->SetParent(hon_Image->rectTransform);

	// 혼
	hon_Image->rectTransform->SetPosition(-850, -300);
	hon_Image->rectTransform->SetSize(70, 70);
	auto honImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Hon.png");
	hon_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(honImageTexture, "Hon");

	hon_Text->rectTransform->SetPosition(200, 0);
	hon_Text->rectTransform->SetSize(330, 50);
	hon_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
	hon_Text->screenTextRenderer->SetFontSize(40);

	hon_Text->screenTextRenderer->SetText(L"x 000") ;

	// 스킬1
	skill1Key_Image->rectTransform->SetParent(skill1_Image->rectTransform);
	skill1Icon_Image->rectTransform->SetParent(skill1_Image->rectTransform);
	skill1CollTimeFilter_Image->rectTransform->SetParent(skill1_Image->rectTransform);

	skill1_Image->rectTransform->SetPosition(0, -132);
	skill1_Image->rectTransform->SetSize(100, 100);
	auto skill1ImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/SkillQ.png");
	skill1_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(skill1ImageTexture, "SkillQ");
	auto skillIconTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/SkillIcon.png");
	skill1Icon_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(skillIconTexture, "SkillIcon");

	skill1Key_Image->rectTransform->SetPosition(0, 50);
	skill1Key_Image->rectTransform->SetSize(50, 50);
	auto skill1KeyImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Q.png");
	skill1Key_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(skill1KeyImageTexture, "Q");
	

	auto skill1CollTimeFilterImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/SkillFilter.png");
	skill1CollTimeFilter_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(skill1CollTimeFilterImageTexture, "SkillFilter");
	skill1CollTimeFilter_Image->imageRenderer->SetAlpha(0.4);
	skill1CollTimeFilter_Image->imageRenderer->fillType = FillType::Vertical;


	// 스킬2
	skill2Key_Image->rectTransform->SetParent(skill2_Image->rectTransform);
	skill2Icon_Image->rectTransform->SetParent(skill2_Image->rectTransform);

	skill2_Image->rectTransform->SetPosition(120, -132);
	skill2_Image->rectTransform->SetSize(100, 100);
	auto skill2ImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/SkillE.png");
	skill2_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(skill2ImageTexture, "SkillE");
	skill2Icon_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(skillIconTexture, "SkillIcon");

	skill2Key_Image->rectTransform->SetPosition(0, 50);
	skill2Key_Image->rectTransform->SetSize(50, 50);
	auto skill2KeyImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/E.png");
	skill2Key_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(skill2KeyImageTexture, "E");

	skill2_Image->imageRenderer->renderMode = RenderMode::UnlitColorTint;
	skill1_Image->imageRenderer->SetColor(0.4, 0.4, 0.4);
	skill2_Image->imageRenderer->SetColor(0.4, 0.4, 0.4);

	// 스킬창 UI
	
	skillWindowClose_Button->rectTransform->SetParent(skillWindowBackGround_Image->rectTransform);
	skillWindow_Image->rectTransform->SetParent(skillWindowBackGround_Image->rectTransform);
	skillWindowSplitter_Image->rectTransform->SetParent(skillWindowBackGround_Image->rectTransform);
	skillWindowBackGroundGradient_Image->rectTransform->SetParent(skillWindowBackGround_Image->rectTransform);
	for (auto& skillButton : skillButtons)
	{
		skillButton->rectTransform->SetParent(skillWindowBackGround_Image->rectTransform);
	}
	skillHonBig_Image->rectTransform->SetParent(skillWindowBackGround_Image->rectTransform);
	skillHon_Image->rectTransform->SetParent(skillWindowBackGround_Image->rectTransform);
	skillHon_Text->rectTransform->SetParent(skillHon_Image->rectTransform);
	skillWindowName_Image->rectTransform->SetParent(skillWindowBackGround_Image->rectTransform);

	skillWindowBackGround_Image->SetActive(false);
	skillWindowBackGround_Image->rectTransform->SetSize(1920, 1080);
	skillWindowBackGroundGradient_Image->rectTransform->SetSize(1920, 1080);

	skillWindowClose_Button->rectTransform->SetSize(1248, 702);
	auto skillWindowClose_ButtonTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/BackButton.png");
	skillWindowClose_Button->imageRenderer->sprite = ResourceManager::Get().CreateSprite(skillWindowClose_ButtonTexture, "BackButton");

	skillWindow_Image->rectTransform->SetSize(1248, 702);
	auto skillWindowImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/SkillWindow.png");
	skillWindow_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(skillWindowImageTexture, "SkillWindow");
	skillWindowSplitter_Image->rectTransform->SetPosition(0, 70);
	skillWindowSplitter_Image->rectTransform->SetSize(700, 400);
	auto skillWindowSplitterImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/SkillWindowSplitter.png");
	skillWindowSplitter_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(skillWindowSplitterImageTexture, "SkillWindowSplitter");
	auto skillWindowBackGroundImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/WindowBackGround.png");
	skillWindowBackGround_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(skillWindowBackGroundImageTexture, "WindowBackGround");
	auto skillWindowBackGroundImageGradientTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/WindowBackGroundGradient.png");
	skillWindowBackGroundGradient_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(skillWindowBackGroundImageGradientTexture, "WindowBackGroundGradient");

	for (int i = 0; i < skillButtons.size(); ++i)
	{
		std::string texturePath = "../Resource/mo/Skill" + std::to_string(i + 1) + ".png";
		std::string spriteName = "Skill" + std::to_string(i + 1);

		auto texture = ResourceManager::Get().CreateTexture2D(texturePath);
		skillButtons[i]->skillIcon_Button->imageRenderer->sprite = ResourceManager::Get().CreateSprite(texture, spriteName);
	}
	skillHon_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(honImageTexture, "Hon");
	skillHonBig_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(honImageTexture, "Hon");


	skillButtons[0]->rectTransform->SetPosition(-400, 160);
	skillButtons[1]->rectTransform->SetPosition(-400, -20);
	skillButtons[2]->rectTransform->SetPosition(-400, -200);
	skillButtons[3]->rectTransform->SetPosition(-50, 160);
	skillButtons[4]->rectTransform->SetPosition(-50, -20);
	skillButtons[5]->rectTransform->SetPosition(-50, -200);
	skillButtons[6]->rectTransform->SetPosition(300, 160);
	skillButtons[7]->rectTransform->SetPosition(300, -20);
	skillButtons[8]->rectTransform->SetPosition(300,-200);

	skillHonBig_Image->rectTransform->SetPosition(0,290);
	skillHonBig_Image->rectTransform->SetSize(50,50);
	skillHon_Image->rectTransform->SetPosition(480,-280);
	skillHon_Image->rectTransform->SetSize(30,30);
	skillHon_Text->rectTransform->SetPosition(120,0);
	skillHon_Text->rectTransform->SetSize(200,35);
	skillHon_Text->screenTextRenderer->SetFontSize(25);
	skillHon_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
	skillHon_Text->screenTextRenderer->SetText(L"x 000");


	// 스킬창 제목
	skillWindowName_Image->rectTransform->SetPosition(0, 350);
	skillWindowName_Image->rectTransform->SetSize(273, 74);
	auto SkillWindowNameGradientTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/SkillWindowName.png");
	skillWindowName_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(SkillWindowNameGradientTexture, "SkillWindowName");

	
	for (UI_Button* btn : pauseCheckButtos)
	{
		btn->button->onClickListeners.AddListener(
			this, std::bind(&PlayUI::CheckPauseUI, this));
	}
	tooltipName = { L"index",L"Ignis",L"Dark",L"Luna",L"Nox"};

	stop_Button->button->onPointEnterListeners.AddListener(
		this, []() {
			GameManager::Get().canUseMouse = false;
		});

	stop_Button->button->onPointExitListeners.AddListener(
		this, [this]() {
			if(pauseWindow->IsActive())
				GameManager::Get().canUseMouse = true;
		});


}

void PlayUI::Update()
{
	if (GameManager::Get().isWave)
	{
		std::wstring timeText = (GameManager::Get().waveTime < 9 ? L"0" : L"") + std::to_wstring(static_cast<int>(std::ceil(GameManager::Get().waveTime)));
		timer_Text->screenTextRenderer->SetText(timeText);
	}

	if (!GameManager::Get().canUseAbsorb)
	{
		float cooltime = GameManager::Get().absorbCoolTime/GameManager::Get().maxabsorbCoolTime;
		skill1CollTimeFilter_Image->imageRenderer->fillAmount = cooltime;
	}

	if (waveInfo_Text->IsActive())
	{
		if (waveInfoTimer < waveIntoTime)
		{
			waveInfoTimer += Time::GetDeltaTime();

			float alpha = 1.0f;

			if (waveInfoTimer < fadeTime) // 페이드 인 구간
			{
				alpha = waveInfoTimer / fadeTime;
			}
			else if (waveInfoTimer > (waveIntoTime - fadeTime)) // 페이드 아웃 구간
			{
				alpha = (waveIntoTime - waveInfoTimer) / fadeTime;
			}
			else // 알파 1 유지 구간
			{
				alpha = 1.0f;
			}

			// clamp 알파값 (0 ~ 1)
			alpha = std::max(0.0f, std::min(1.0f, alpha));
			waveInfo_Text->screenTextRenderer->SetAlpha(alpha);
		}
		else
		{
			waveInfoTimer = 0;
			waveInfo_Text->screenTextRenderer->SetAlpha(0);
			waveInfo_Text->SetActive(false);
		}
	}

	if (tooltip1->IsActive())
	{
		if (tooltipInfoTimer < tooltipInfoTime)
		{
			tooltipInfoTimer += Time::GetDeltaTime();

			float alpha = 1.0f;

			if (tooltipInfoTimer < fadeTime) // 페이드 인
			{
				alpha = tooltipInfoTimer / fadeTime;
			}
			else if (tooltipInfoTimer > (tooltipInfoTime - fadeTime)) // 페이드 아웃
			{
				alpha = (tooltipInfoTime - tooltipInfoTimer) / fadeTime;
			}
			else // 알파 1 유지
			{
				alpha = 1.0f;
			}

			alpha = std::max(0.0f, std::min(1.0f, alpha));
			tooltip1->tooltipBackGround_Image->imageRenderer->SetAlpha(alpha);
			if (tooltip2->IsActive()) tooltip2->tooltipBackGround_Image->imageRenderer->SetAlpha(alpha);
		}
		else
		{
			tooltipInfoTimer = 0;
			tooltip1->tooltipBackGround_Image->imageRenderer->SetAlpha(0);
			tooltip1->SetActive(false);
			if (tooltip2->IsActive())
			{
				tooltip2->tooltipBackGround_Image->imageRenderer->SetAlpha(0);
				tooltip2->SetActive(false);
			}
		}
	}

	// TODOMO : 아래 입력 삭제 



	if (((Input::GetKeyDown(VK_ESCAPE)&& skillWindowBackGround_Image->IsActive())||Input::GetKeyDown(VK_TAB)))
	{
		if (pauseWindow->IsActive() || chat_Image->IsActive())
			return;
		if (skillWindowBackGround_Image->IsActive())
		{
			GameManager::Get().canUseMouse = true;
			skillWindowBackGround_Image->SetActive(false);
		}
		else
		{
			GameManager::Get().canUseMouse = false;
			skillWindowBackGround_Image->SetActive(true);
			sfxSource->SetClip(sfxClip_SkillUI);
			sfxSource->Play();
		}
	}

	if (Input::GetKeyDown('R'))
	{
		GameManager::Get().SkillReset();
	}

	if (Input::GetKeyDown(VK_F3))
	{
		GameManager::Get().ChangeHonCount(100);
	}

}

void PlayUI::Destroyed()
{

}

void PlayUI::ClickChatButton() {

	switch (GameManager::Get().waveCount)
	{
	case 3:
		GameObject::Find("MoonShadow")->GetComponent<MoonShadowController>()->DirectingBossWave();
		return;
	case 4:
		SceneManager::Get().ChangeScene(EclipseApp::END);// TODOMO : 추후 크레딧으로 변경
		return;
	}
	WaveStartData();
}

void PlayUI::BossIntroEnd()
{
	bossHP->SetActive(true);
	WaveStartData();
}

void PlayUI::WaveStartData()
{
	GameManager::Get().WaveStart();
	chat_Button->SetActive(false);
	chat_Image->SetActive(false);
	GameManager::Get().canUseMouse = true;
	StartWaveInfo(GameManager::Get().waveCount);
	quest->RefreshQuestText();
	quest->RefreshQuestCountText(0);
	questCount_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	if (skillWindowBackGround_Image->IsActive()) skillWindowBackGround_Image->SetActive(false);
	hon_Image->SetActive(true);
	stop_Button->SetActive(true);
	quest_Image->SetActive(true);
	GameObject::Find("InGameCamera")->GetComponent<CameraController>()->ZoomOutFromPlayer();
}

void PlayUI::StartWaveInfo(int waveNumber)
{
	std::wstring waveText;
	if (waveNumber < 5)
	{
		waveText = L"공세 " + std::to_wstring(waveNumber) + L"막";
		bgmSource->SetClip(bgmClip_Wave);
	}
	else
	{
		waveText = L"Boss";
		bgmSource->SetClip(bgmClip_Boss);
	}
	waveInfo_Text->screenTextRenderer->SetText(waveText);
	
	waveInfoTimer = 0;
	waveInfo_Text->SetActive(true);
	waveInfo_Text->screenTextRenderer->SetAlpha(0);

	tooltipInfoTimer = 0;

	tooltip1->SetActive(true);
	tooltip1->ChangeInfo(tooltipName[waveNumber]);
	tooltip1->tooltipBackGround_Image->imageRenderer->SetAlpha(0);

	if (waveNumber == 1)
	{
		tooltip2->SetActive(true);
		tooltip2->ChangeInfo(L"Umbra");
		tooltip2->tooltipBackGround_Image->imageRenderer->SetAlpha(0);
	}
	bgmSource->Play();
}

void PlayUI::AllSkillCheat()
{
	for (auto& skillButton : skillButtons)
	{
		skillButton->skillIcon_Button->imageRenderer->renderMode = RenderMode::Unlit;
		skillButton->RefreshCurrentLevelText();
		skillButton->RefreshLevelUPHonCountText();
	}
}

void PlayUI::ResetAllSkillCheat()
{
	for (auto& skillButton : skillButtons)
	{
		skillButton->skillIcon_Button->imageRenderer->renderMode = RenderMode::UnlitColorTint;
		skillButton->RefreshCurrentLevelText();
		skillButton->RefreshLevelUPHonCountText();
	}
}

void PlayUI::PlayerInteraction()
{
	if (chat_Image->IsActive())
	{
		chat->NextChat();
	}
}

// 혼 개수 추가 호출 함수
void PlayUI::ChangeHonCountText()
{
	int count = GameManager::Get().honCount;

	std::wstringstream ss;
	ss << L"x " << std::setw(3) << std::setfill(L'0') << count;

	hon_Text->screenTextRenderer->SetText(ss.str());
	skillHon_Text->screenTextRenderer->SetText(ss.str());
}

void PlayUI::ActivateAbsorb()
{
	skill1CollTimeFilter_Image->SetActive(false);
	/*skill1_Image->imageRenderer->renderMode = RenderMode::Unlit;
	skill1CollTime_Text->SetActive(false);*/
}

void PlayUI::DeactivateAbsorb()
{
	skill1CollTimeFilter_Image->SetActive(true);
	/*skill1_Image->imageRenderer->renderMode = RenderMode::UnlitColorTint;
	skill1CollTime_Text->SetActive(true);*/
}

void PlayUI::CheckPauseUI()
{
	bool check = !pauseWindow->IsActive();
	pauseWindow->SetActive(check);
	if (check)
		Time::SetTimeScale(0);
	else
	{
		Time::SetTimeScale(1);
		GameManager::Get().canUseMouse = true;
	}
	ButtonClickSound();
}

void PlayUI::SkillActiveSound()
{
	sfxSource->SetClip(sfxClip_SkillActive);
	sfxSource->Play();
}

void PlayUI::ButtonEnterSound()
{
	sfxSource->SetClip(sfxClip_Button1);
	sfxSource->Play();
}

void PlayUI::ButtonClickSound()
{
	sfxSource->SetClip(sfxClip_Button2);
	sfxSource->Play();
}