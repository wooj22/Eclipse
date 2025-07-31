#include "PlayerMove.h"
#include "PlayUI.h"
#include "Chat.h"
#include "Quest.h"
#include "../Direct2D_EngineLib/Time.h"
#include "GameManager.h"

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
		if (GameManager::Get().g_playUI->ChatActiveCheck())
		{
			if (!GameManager::Get().g_playUI->chat_Text->GetComponent<Chat>()->GetFinished())
				GameManager::Get().g_playUI->chat_Text->GetComponent<Chat>()->AddChatCount();
		}
	}

	if (Input::GetKeyDown(VK_RETURN))
	{
		GameManager::Get().isWave = false;
	}

	if (Input::GetKeyDown(VK_F1))
	{
		GameManager::Get().g_playUI->chat->SetCondition(ChatCondition::Success);
	}

	if (Input::GetKeyDown(VK_F2))
	{
		GameManager::Get().g_playUI->chat->SetCondition(ChatCondition::Fail);
	}
}
void PlayerMove::OnTriggerStay(ICollider* other)
{
	if (other->gameObject->name == "NPC" && !GameManager::Get().g_playUI->ChatActiveCheck() 
		&& !GameManager::Get().isWave && Input::GetKey('F'))
	{
			GameManager::Get().g_playUI->ChatSetActive(true);
	}
}
