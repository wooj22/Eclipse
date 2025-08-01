#define NOMINMAX
#include "PlayUI.h"
#include "Chat.h"
#include "Quest.h"
#include <algorithm>

void PlayUI::Awake()
{  

}

void PlayUI::SceneStart()
{
	// 웨이브 타이머 UI
	timer_Text->rectTransform->SetPosition(0, 500);
	timer_Text->rectTransform->SetSize(500, 40);
	timer_Text->screenTextRenderer->SetText(L"추후 타이머 연결");
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
	quest_Image->rectTransform->SetPosition(800, 0);
	quest_Image->rectTransform->SetSize(300, 500);
	auto questImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Quest.png");
	quest_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(questImageTexture, "Quest");
	quest_Text->rectTransform->SetSize(200, 0);
	quest_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
	quest_Text->screenTextRenderer->SetVerticalAlign(TextVerticalAlign::Top);
	quest_Text->AddComponent<Quest>();

	// 대화창 UI
	chat_Text->rectTransform->SetParent(chat_Image->rectTransform);
	chat_Button->rectTransform->SetParent(chat_Image->rectTransform);
	chat_Button->button->onClickListeners.AddListener(
		this, std::bind(&PlayUI::ClickChatButton, this));

	chat_Image->SetActive(false);
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
	skill2_Text->rectTransform->SetParent(hon_Image->rectTransform);

	// 혼
	hon_Image->rectTransform->SetPosition(-850, -300);
	hon_Image->rectTransform->SetSize(50, 50);
	auto honImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Hon.png");
	hon_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(honImageTexture, "Hon");

	hon_Text->rectTransform->SetPosition(200, 0);
	hon_Text->rectTransform->SetSize(330, 50);
	hon_Text->screenTextRenderer->SetHorizontalAlign(TextHorizontalAlign::Left);
	hon_Text->screenTextRenderer->SetFontSize(50);

	// 스킬1
	skill1_Image->rectTransform->SetPosition(20, -125);
	skill1_Image->rectTransform->SetSize(200, 200);
	auto skill1ImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Skill1.png");
	skill1_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(skill1ImageTexture, "Skill1");

	/*skill1_Text->rectTransform->SetPosition(-830, -300);
	skill1_Text->rectTransform->SetSize(100, 50);
	skill1_Text->screenTextRenderer->SetText(L"x 1");
	skill1_Text->screenTextRenderer->SetFontSize(50);*/

	// 스킬2
	skill2_Image->rectTransform->SetPosition(200, -125);
	skill2_Image->rectTransform->SetSize(200, 200);
	auto skill2ImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Skill2.png");
	skill2_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(skill2ImageTexture, "Skill2");

	/*skill2_Text->rectTransform->SetPosition(-830, -300);
	skill2_Text->rectTransform->SetSize(100, 50);
	skill2_Text->screenTextRenderer->SetText(L"x 1");
	skill2_Text->screenTextRenderer->SetFontSize(50);*/

}


void PlayUI::Update()
{

	hon_Text->screenTextRenderer->SetText(L"x " + std::to_wstring(GameManager::Get().honCount));

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
}

void PlayUI::Destroyed()
{

}

void PlayUI::ClickChatButton() {
	GameManager::Get().isWave = true;
	GameManager::Get().waveCount++;
	chat_Button->SetActive(false);
	chat_Image->SetActive(false);
	StartWaveInfo(GameManager::Get().waveCount);
}

void PlayUI::StartWaveInfo(int waveNumber)
{
	waveInfo_Text->screenTextRenderer->SetText(L"Wave " + std::to_wstring(waveNumber));
	
	waveInfoTimer = 0;
	waveInfo_Text->SetActive(true);
	waveInfo_Text->screenTextRenderer->SetAlpha(0);

	tolltipInfoTimer = 0;
	tooltip_Image->SetActive(true);
	tooltip_Image->imageRenderer->SetAlpha(0);
}