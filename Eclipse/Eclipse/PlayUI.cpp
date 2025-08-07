#define NOMINMAX
#include "PlayUI.h"
#include "Chat.h"
#include "Quest.h"
#include "EclipseApp.h"
#include <algorithm>
#include "../Direct2D_EngineLib/Input.h"

void PlayUI::Awake()
{  

}

void PlayUI::SceneStart()
{
	//해당 씬에 게임 오브젝트 생성
	timer_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	stop_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	quest_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	quest_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	questCount_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	chat_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	chat_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	chat_Button = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Button>();
	hon_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	hon_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	skill1_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	skill1_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	skill2_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	waveInfo_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	tooltip_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	skillWindow_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::KnockbackDistanceUp));
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::DoubleJump));
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::WallJump));
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::SkillCooldownDown));
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::JumpAttackExtra));
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::FastFall));
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::MoveSpeedUp));
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::AttackRangeUp));
	skillButtons.push_back(SceneManager::Get().GetCurrentScene()->CreateObject<SkillWindowButton>({ 0,0 }, nullptr, SkillType::Dash));
	skillHon_Image = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Image>();
	skillHon_Text = SceneManager::Get().GetCurrentScene()->CreateObject<UI_Text>();
	
	// 웨이브 타이머 UI
	timer_Text->rectTransform->SetPosition(0, 500);
	timer_Text->rectTransform->SetSize(500, 40);
	timer_Text->screenTextRenderer->SetFontSize(50);

	waveInfo_Text->rectTransform->SetPosition(0, 400);
	waveInfo_Text->rectTransform->SetSize(500, 40);
	waveInfo_Text->screenTextRenderer->SetFontSize(50);
	waveInfo_Text->SetActive(false);

	// 일시 정지 버튼
	stop_Button->rectTransform->SetPosition(870, 480);
	stop_Button->rectTransform->SetSize(50, 50);
	auto stopButtonTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/StopButton.png");
	stop_Button->imageRenderer->sprite = ResourceManager::Get().CreateSprite(stopButtonTexture, "StopButton");

	// 툴팁 UI
	tooltip_Image->rectTransform->SetPosition(-700, 75);
	tooltip_Image->rectTransform->SetSize(450, 150);
	auto tooltipTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Tooltip.png");
	tooltip_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(tooltipTexture, "Tooltip");
	tooltip_Image->SetActive(false);


	
	// 퀘스트 창 UI
	quest_Text->rectTransform->SetParent(quest_Image->rectTransform);
	questCount_Text->rectTransform->SetParent(quest_Image->rectTransform);
	quest_Image->rectTransform->SetPosition(800, 0);
	quest_Image->rectTransform->SetSize(300, 500);
	auto questImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Quest.png");
	quest_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(questImageTexture, "Quest");
	quest_Text->rectTransform->SetSize(200, 0);
	quest_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
	quest_Text->screenTextRenderer->SetVerticalAlign(TextVerticalAlign::Top);
	questCount_Text->rectTransform->SetPosition(0, -50);
	questCount_Text->rectTransform->SetSize(200, 0);
	questCount_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
	questCount_Text->screenTextRenderer->SetVerticalAlign(TextVerticalAlign::Top);
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
	chat_Image->rectTransform->SetSize(1500, 200);
	auto chatImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Chat.png");
	chat_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(chatImageTexture, "Chat");
	
	chat_Text->rectTransform->SetSize(700, 50);
	chat_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
	chat_Text->screenTextRenderer->SetVerticalAlign(TextVerticalAlign::Top);

	chat = chat_Text->AddComponent<Chat>();
	chat_Button->rectTransform->SetPosition(600, -80);
	chat_Button->rectTransform->SetSize(100, 50);
	auto chatButtonTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/ChatButton.png");
	chat_Button->imageRenderer->sprite = ResourceManager::Get().CreateSprite(chatButtonTexture, "ChatButton");


	// 혼 & 스킬 UI
	hon_Text->rectTransform->SetParent(hon_Image->rectTransform);
	skill1_Image->rectTransform->SetParent(hon_Image->rectTransform);
	skill1_Text->rectTransform->SetParent(hon_Image->rectTransform);
	skill2_Image->rectTransform->SetParent(hon_Image->rectTransform);

	// 혼
	hon_Image->rectTransform->SetPosition(-850, -300);
	hon_Image->rectTransform->SetSize(50, 50);
	auto honImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Hon.png");
	hon_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(honImageTexture, "Hon");

	hon_Text->rectTransform->SetPosition(200, 0);
	hon_Text->rectTransform->SetSize(330, 50);
	hon_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
	hon_Text->screenTextRenderer->SetFontSize(50);

	hon_Text->screenTextRenderer->SetText(L"x 0") ;

	// 스킬1
	skill1_Text->rectTransform->SetParent(skill1_Image->rectTransform);

	skill1_Image->rectTransform->SetPosition(20, -125);
	skill1_Image->rectTransform->SetSize(200, 200);
	auto skill1ImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Skill1.png");
	skill1_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(skill1ImageTexture, "Skill1");
	skill1_Text->rectTransform->SetSize(150, 150);
	skill1_Text->screenTextRenderer->SetFontSize(50);

	// 스킬2
	skill2_Image->rectTransform->SetPosition(200, -125);
	skill2_Image->rectTransform->SetSize(200, 200);
	auto skill2ImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Skill2.png");
	skill2_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(skill2ImageTexture, "Skill2");

	skill2_Image->imageRenderer->renderMode = RenderMode::UnlitColorTint;
	skill1_Image->imageRenderer->SetColor(0.4, 0.4, 0.4);
	skill2_Image->imageRenderer->SetColor(0.4, 0.4, 0.4);

	// 스킬창 UI
	for (auto& skillButton : skillButtons)
	{
		skillButton->rectTransform->SetParent(skillWindow_Image->rectTransform);
	}
	skillHon_Image->rectTransform->SetParent(skillWindow_Image->rectTransform);
	skillHon_Text->rectTransform->SetParent(skillWindow_Image->rectTransform);

	skillWindow_Image->SetActive(false);
	skillWindow_Image->rectTransform->SetSize(1248, 702);
	auto skillWindowImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/SkillWindow.png");
	skillWindow_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(skillWindowImageTexture, "SkillWindow");

	for (int i = 0; i < skillButtons.size(); ++i)
	{
		std::string texturePath = "../Resource/mo/Skill" + std::to_string(i + 1) + ".png";
		std::string spriteName = "Skill" + std::to_string(i + 1);

		auto texture = ResourceManager::Get().CreateTexture2D(texturePath);
		skillButtons[i]->skillIcon_Button->imageRenderer->sprite = ResourceManager::Get().CreateSprite(texture, spriteName);
	}
	skillHon_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(honImageTexture, "Hon");


	skillButtons[0]->rectTransform->SetPosition(-50, -50);
	skillButtons[1]->rectTransform->SetPosition(-50, 100);
	skillButtons[2]->rectTransform->SetPosition(-50, 250);
	skillButtons[3]->rectTransform->SetPosition(-450, -200);
	skillButtons[4]->rectTransform->SetPosition(-450, -50);
	skillButtons[5]->rectTransform->SetPosition(-450, 100);
	skillButtons[6]->rectTransform->SetPosition(300, -200);
	skillButtons[7]->rectTransform->SetPosition(300, -50);
	skillButtons[8]->rectTransform->SetPosition(300,100);

	skillHon_Image->rectTransform->SetPosition(-100,-250);
	skillHon_Text->rectTransform->SetPosition(200,-250);
	skillHon_Text->rectTransform->SetSize(500,100);
	skillHon_Text->screenTextRenderer->SetFontSize(50);
	skillHon_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
	skillHon_Text->screenTextRenderer->SetText(L"x 0");
}

void PlayUI::Update()
{
	if (GameManager::Get().isWave)
	{
		std::wstring timeText = (GameManager::Get().waveTime < 9 ? L"0" : L"") + std::to_wstring(static_cast<int>(std::ceil(GameManager::Get().waveTime)));
		timer_Text->screenTextRenderer->SetText(timeText);
	}

	if (!GameManager::Get().canUseAbsorb)
		skill1_Text->screenTextRenderer->SetText(std::to_wstring(static_cast<int>(std::ceil(GameManager::Get().absorbCoolTime))));

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

	if (tooltip_Image->IsActive())
	{
		if (tolltipInfoTimer < tolltipInfoTime)
		{
			tolltipInfoTimer += Time::GetDeltaTime();

			float alpha = 1.0f;

			if (tolltipInfoTimer < fadeTime) // 페이드 인
			{
				alpha = tolltipInfoTimer / fadeTime;
			}
			else if (tolltipInfoTimer > (tolltipInfoTime - fadeTime)) // 페이드 아웃
			{
				alpha = (tolltipInfoTime - tolltipInfoTimer) / fadeTime;
			}
			else // 알파 1 유지
			{
				alpha = 1.0f;
			}

			alpha = std::max(0.0f, std::min(1.0f, alpha));
			tooltip_Image->imageRenderer->SetAlpha(alpha);
		}
		else
		{
			tolltipInfoTimer = 0;
			tooltip_Image->imageRenderer->SetAlpha(0);
			tooltip_Image->SetActive(false);
		}
	}

	// TODOMO : 아래 입력 삭제 

	if (Input::GetKeyDown('Q') && GameManager::Get().canUseAbsorb)
	{
		GameManager::Get().absorbCoolTime = 9;
		GameManager::Get().canUseAbsorb = false;
		GameManager::Get().canUseRelease = true;
	}

	if (Input::GetKeyDown(VK_TAB))
	{
		if (skillWindow_Image->IsActive())
		{
			skillWindow_Image->SetActive(false);
		}
		else
		{
			skillWindow_Image->SetActive(true);
		}
	}

	if (Input::GetKeyDown('E') && GameManager::Get().canUseRelease)
	{
		GameManager::Get().canUseRelease = false;
	}

	if (GameManager::Get().absorbCoolTime > 0)
		GameManager::Get().absorbCoolTime -= Time::GetDeltaTime();
	else
		GameManager::Get().canUseAbsorb = true;
	

}

void PlayUI::Destroyed()
{

}

void PlayUI::ClickChatButton() {
	if (GameManager::Get().waveCount == 4)
	{
		SceneManager::Get().ChangeScene(EclipseApp::END);// TODOMO : 추후 크레딧으로 변경
		return;
	}
	GameManager::Get().WaveStart();
	chat_Button->SetActive(false);
	chat_Image->SetActive(false);
	StartWaveInfo(GameManager::Get().waveCount);
	quest->RefreshQuestText();
	quest->RefreshQuestCountText(0);
	questCount_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	if (skillWindow_Image->IsActive()) skillWindow_Image->SetActive(false);
}

void PlayUI::StartWaveInfo(int waveNumber)
{
	std::wstring waveText = waveNumber < 4 ? L"Wave " + std::to_wstring(waveNumber) : L"Boss" ;
	waveInfo_Text->screenTextRenderer->SetText(waveText);
	
	waveInfoTimer = 0;
	waveInfo_Text->SetActive(true);
	waveInfo_Text->screenTextRenderer->SetAlpha(0);

	tolltipInfoTimer = 0;
	tooltip_Image->SetActive(true);
	tooltip_Image->imageRenderer->SetAlpha(0);
}

void PlayUI::AllSkillCheat()
{
	for (auto& skillButton : skillButtons)
	{
		skillButton->skillIcon_Button->imageRenderer->renderMode = RenderMode::Unlit;
		skillButton->RefreshText();
	}
}

void PlayUI::ResetAllSkillCheat()
{
	for (auto& skillButton : skillButtons)
	{
		skillButton->skillIcon_Button->imageRenderer->renderMode = RenderMode::UnlitColorTint;
		skillButton->RefreshText();
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
	hon_Text->screenTextRenderer->SetText(L"x " + std::to_wstring(GameManager::Get().honCount));
	skillHon_Text->screenTextRenderer->SetText(L"x " + std::to_wstring(GameManager::Get().honCount));
}

void PlayUI::ActivateAbsorb()
{
	skill1_Image->imageRenderer->renderMode = RenderMode::Unlit;
	skill1_Text->SetActive(false);
}

void PlayUI::DeactivateAbsorb()
{
	skill1_Image->imageRenderer->renderMode = RenderMode::UnlitColorTint;
	skill1_Text->SetActive(true);
}
