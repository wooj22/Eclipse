#include "PlayerMove.h"
#include "NPC.h"


void PlayerMove::OnEnable()
{
	transform = gameObject->GetComponent<Transform>();
}

void PlayerMove::Update()
{
	inputX = Input::GetAxisHorizontal();
	inputY = Input::GetAxisVertical();

	if (Input::GetKey('A') || Input::GetKey('D'))
	{
		transform->Translate(Vector2(inputX, inputY).Normalized() * speed);
	}
}

void PlayerMove::OnCollisionStay(ICollider* other, const ContactInfo& contact)
{
	if (other->gameObject->name == "NPC")
	{
		if (Input::GetKeyDown('F'))
		{
			NPC* npc = dynamic_cast<NPC*>(other->gameObject);
			if (npc)
			{
				npc->interationListeners.Invoke();
			}
		}
	}
}
