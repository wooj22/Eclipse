#include "PlayUI.h"

void PlayUI::Awake()  
{  
	//questImage->GetComponent<Transform>()->SetParent(questText->GetComponent<Transform>());  
	//chatImage->GetComponent<Transform>()->SetParent(chatText->GetComponent<Transform>());  
}

void PlayUI::SceneStart()
{
	stopButton->rectTransform->SetPosition(870, 480);
	stopButton->rectTransform->SetSize(50, 50);
	auto stopButtonTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/StopButton.png");
	stopButton->imageRenderer->sprite = ResourceManager::Get().CreateSprite(stopButtonTexture, "StopButton");

	//questImage->rectTransform->SetPosition(0, -300);
	//questImage->rectTransform->SetSize(0, 400);

	chatImage->rectTransform->SetPosition(0, -400);
	chatImage->rectTransform->SetSize(500, 200);
	auto chatImageTexture = ResourceManager::Get().CreateTexture2D("../Resource/mo/StopButton.png");
	chatImage->imageRenderer->sprite = ResourceManager::Get().CreateSprite(chatImageTexture, "StopButton");

}

void PlayUI::Update()
{

}

void PlayUI::Destroyed()
{

}