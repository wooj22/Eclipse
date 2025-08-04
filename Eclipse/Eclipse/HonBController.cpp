#include "HonBController.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Time.h"
#include "HonB.h"
#include "HonAController.h"

// script component cycle
void HonBController::Awake()
{
	tr = gameObject->transform;
	collider = gameObject->GetComponent<CircleCollider>();

	playerTr = GameObject::Find("Player")->GetComponent<Transform>();
}

void HonBController::Start()
{
	tr->SetScale(size, size);
}

void HonBController::Update()
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

void HonBController::OnDestroy()
{

}

// trigger event
void HonBController::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
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
			HonAController* otherController = otherGameObject->GetComponent<HonAController>();
			direction = (tr->GetWorldPosition() - otherGameObject->transform->GetWorldPosition()).Normalized();
		}
		else if (honType == "HonB")
		{
			SetSize(size * 0.7);
			SetDescentSpeed(descentSpeed * 1.2);
			SetDirection(Vector2::down);

			GameObject* newHonB = Instantiate<HonB>(tr->GetWorldPosition() + Vector2(100,0));
			HonBController* controller = newHonB->GetComponent<HonBController>();
			controller->SetSize(size);
			controller->SetDescentSpeed(descentSpeed * 1.2);
		}
		
		// collider on
		collider->SetEnabled(true);
	}
}