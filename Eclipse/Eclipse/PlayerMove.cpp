#include "PlayerMove.h"
#include "PlayUI.h"
//#include "NPCScript.h"
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

	if (Input::GetKeyDown('F') && PlayUI::Get().ChatActiveCheck())
	{
		PlayUI::Get().CloseChat();
	}
}
void PlayerMove::OnTriggerStay(ICollider* other)
{
	if (other->gameObject->name == "NPC" && !PlayUI::Get().ChatActiveCheck())
	{
		if (Input::GetKey('F'))
		{
			//other->gameObject->GetComponent<NPCScript>()->Interaction();
			PlayUI::Get().OpenChat();
		}
	}
}
