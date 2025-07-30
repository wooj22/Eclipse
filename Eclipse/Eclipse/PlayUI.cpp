#include "PlayUI.h"
#include "Chat.h"
#include "Quest.h"

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

	// 일시 정지 버튼
	stop_Button->rectTransform->SetPosition(870, 480);
	stop_Button->rectTransform->SetSize(50, 50);
	auto stopButtonTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/StopButton.png");
	stop_Button->imageRenderer->sprite = ResourceManager::Get().CreateSprite(stopButtonTexture, "StopButton");

	// 퀘스트 창 UI
	quest_Text->rectTransform->SetParent(quest_Image->rectTransform);
	quest_Image->rectTransform->SetPosition(800, 0);
	quest_Image->rectTransform->SetSize(300, 500);
	auto questImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Quest.png");
	quest_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(questImageTexture, "Quest");
	//chat_Text->rectTransform->SetPosition(0, -100);
	quest_Text->rectTransform->SetSize(100, 180);
	quest_Text->screenTextRenderer->SetText(L"퀘스트창 텍스트 예시.");
	quest_Text->AddComponent<Quest>();

	// 대화창 UI
	chat_Text->rectTransform->SetParent(chat_Image->rectTransform);
	//chat_Button->rectTransform->SetParent(chat_Image->rectTransform);
	chat_Button->button->onClickListeners.AddListener(
		this, std::bind(&PlayUI::ClickChatButton, this));
	chat_Image->SetActive(false);
	chat_Image->rectTransform->SetPosition(0, -400);
	chat_Image->rectTransform->SetSize(1500, 200);
	auto chatImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/Chat.png");
	chat_Image->imageRenderer->sprite = ResourceManager::Get().CreateSprite(chatImageTexture, "Chat");
	chat_Text->rectTransform->SetSize(1200, 180);
	chat_Text->screenTextRenderer->SetText(L"대화창 텍스트 예시");
	chat = chat_Text->AddComponent<Chat>();
	chat_Button->rectTransform->SetPosition(600, -480);
	//chat_Button->rectTransform->SetPosition(600, -80);
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

	hon_Text->rectTransform->SetPosition(70, 0);
	hon_Text->rectTransform->SetSize(100, 50);
	hon_Text->screenTextRenderer->SetText(L"x 1");
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

}

void PlayUI::Destroyed()
{

}
