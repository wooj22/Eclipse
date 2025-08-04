#include "HonAController.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Time.h"
#include "HonBController.h"

// script component cycle
void HonAController::Awake()
{
	tr = gameObject->transform;
	collider = gameObject->GetComponent<CircleCollider>();

	playerTr = GameObject::Find("Player")->GetComponent<Transform>();
}

void HonAController::Start()
{
	tr->SetScale(size, size);
}

void HonAController::Update()
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
		tr->Translate(Vector2::down * descentSpeed * Time::GetDeltaTime());
	}
}

void HonAController::OnDestroy()
{

}

// trigger event
void HonAController::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
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

		// collider off
		collider->SetEnabled(false);

		if (honType == "HonA")
		{
			// controller
			HonAController* otherController = otherGameObject->GetComponent<HonAController>();

			// Size를 기준으로 합체 주체 결정
			if (size >= otherController->GetSize())
			{
				if(!otherGameObject->IsDestroyed()) otherGameObject->Destroy();
				SetSize(size * 1.5);
				SetDescentSpeed(descentSpeed * 0.6);
			}
			else
			{
				otherController->SetSize(otherController->GetSize() * 1.5);
				otherController->SetDescentSpeed(otherController->GetSDescentpeed() * 0.6);
				if (!this->gameObject->IsDestroyed()) this->gameObject->Destroy();
			}
		}
		else if (honType == "HonB")
		{
			HonBController* otherController = otherGameObject->GetComponent<HonBController>();
			direction = (tr->GetWorldPosition() - otherGameObject->transform->GetWorldPosition()).Normalized();
		}


		// collider on
		collider->SetEnabled(true);
	}
}