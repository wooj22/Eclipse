#include "HonBController.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "HonB.h"

// script component cycle
void HonBController::Awake()
{
	tr = gameObject->transform;
	sr = gameObject->GetComponent<SpriteRenderer>();
	rb = gameObject->GetComponent<Rigidbody>();
	collider = gameObject->GetComponent<CircleCollider>();

	playerTr = GameObject::Find("Player")->GetComponent<Transform>();
}

void HonBController::Start()
{
	tr->SetScale(size, size);
}

void HonBController::Update()
{
	if (isPlayerCollision)
	{
		pushBackDeltaTime += Time::GetDeltaTime();
		tr->Translate(direction * collisionSpeed * Time::GetDeltaTime());

		if (pushBackDeltaTime >= pushBackTime)
		{
			isPlayerCollision = false;
			pushBackDeltaTime = 0;
		}
	}
	else
	{
		tr->Translate(direction * descentSpeed * Time::GetDeltaTime());
	}
}

void HonBController::OnDestroy()
{

}

// trigger event
void HonBController::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
{
	// player collision
	if (!isPlayerCollision && other->gameObject->name == "Player")
	{
		direction = (tr->GetWorldPosition() - playerTr->GetWorldPosition()).Normalized();
		isPlayerCollision = true;
	}

	// hon collision
	if (other->gameObject->tag == "Hon")
	{
		// other gameobject
		GameObject* otherGameObject = other->gameObject;
		string honType = otherGameObject->name;

		// collider off
		collider->SetEnabled(false);

		if (honType == "HonA")
		{
			
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
			controller->SetDirection(Vector2::down);
		}
		else if (honType == "HonC")
		{

		}
		else if (honType == "HonD")
		{

		}
	}
}

void HonBController::OnTriggerStay(ICollider* other, const ContactInfo& contact)
{

}

void HonBController::OnTriggerExit(ICollider* other, const ContactInfo& contact)
{

}