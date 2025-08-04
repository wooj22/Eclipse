#include "HonDController.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Time.h"

// script component cycle
void HonDController::Awake()
{
	tr = gameObject->transform;
	collider = gameObject->GetComponent<CircleCollider>();
	playerTr = GameObject::Find("Player")->GetComponent<Transform>();
}

void HonDController::Update()
{
	if (isCollisionMoving)
	{
		// collision move
		pushBackDeltaTime += Time::GetDeltaTime();
		tr->Translate(direction * collisionSpeed * Time::GetDeltaTime());

		// move end
		if (pushBackDeltaTime >= pushBackTime)
		{
			isCollisionMoving = false;
			pushBackDeltaTime = 0;
		}
	}
	else
	{
		// descent move
		tr->Translate(descentDirection * descentSpeed * Time::GetDeltaTime());
	}
}


// trigger event
void HonDController::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
{
	// player collision
	if (other->gameObject->name == "PlayerAttackArea")
	{
		// collision move start (reset)
		isCollisionMoving = true;
		pushBackDeltaTime = 0;

		// direction
		direction = (tr->GetWorldPosition() - playerTr->GetWorldPosition()).Normalized();
	}

	// hon collision
	if (other->gameObject->tag == "Hon")
	{
		// other gameobject
		GameObject* otherGameObject = other->gameObject;
		if (otherGameObject->IsDestroyed()) return;
		string honType = otherGameObject->name;

		// collision move start (reset)
		isCollisionMoving = true;
		pushBackDeltaTime = 0;

		// destroy
		otherGameObject->Destroy();
		this->gameObject->Destroy();
	}
}
