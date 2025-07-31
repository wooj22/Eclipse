#include "PlayerMove.h"
#include "PlayUI.h"
#include "Chat.h"
//#include "Quest.h"
#include "../Direct2D_EngineLib/Time.h"

void PlayerMove::OnEnable()
{
	transform = gameObject->transform;
	rigid = gameObject->GetComponent<Rigidbody>();
}

void PlayerMove::Update()
{
	inputX = Input::GetAxisHorizontal();
	inputY = Input::GetAxisVertical();
	if (inputX == 0) rigid->velocity.x = 0; // 수평 이동이 없을 때 속도 초기화
	if (inputY == 0) rigid->velocity.y = 0; // 수평 이동이 없을 때 속도 초기화
	rigid->velocity.x += inputX * speed * Time::GetDeltaTime();
	rigid->velocity.y += inputY * speed * Time::GetDeltaTime();

	if (Input::GetKeyDown('F'))
	{
		if (PlayUI::Get().ChatActiveCheck())
		{
			if (!PlayUI::Get().chat_Text->GetComponent<Chat>()->GetFinished())
				PlayUI::Get().chat_Text->GetComponent<Chat>()->AddChatCount();
		}
	}

	if (Input::GetKeyDown(VK_RETURN))
	{
		PlayUI::Get().isWave = false;
	}

	if (Input::GetKeyDown(VK_F1))
	{
		PlayUI::Get().chat->SetCondition(ChatCondition::Success);
	}

	if (Input::GetKeyDown(VK_F2))
	{
		PlayUI::Get().chat->SetCondition(ChatCondition::Fail);
	}
}
void PlayerMove::OnTriggerStay(ICollider* other)
{
	if (other->gameObject->name == "NPC" && !PlayUI::Get().ChatActiveCheck() 
		&& !PlayUI::Get().isWave && Input::GetKey('F'))
	{
			PlayUI::Get().ChatSetActive(true);
	}
}
