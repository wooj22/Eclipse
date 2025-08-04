#include "HonAController.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "HonA.h"

// script component cycle
void HonAController::Awake()
{
	tr = gameObject->transform;
	sr = gameObject->GetComponent<SpriteRenderer>();
	rb = gameObject->GetComponent<Rigidbody>();
	collider = gameObject->GetComponent<CircleCollider>();

	playerTr = GameObject::Find("Player")->GetComponent<Transform>();
}

void HonAController::Start()
{
	tr->SetScale(size, size);
}

void HonAController::Update()
{
	if (isPlayerCollision)
	{
		pushBackDeltaTime += Time::GetDeltaTime();
		tr->Translate(direction * speed * Time::GetDeltaTime());

		if (pushBackDeltaTime >= pushBackTime)
		{
			isPlayerCollision = false;
			pushBackDeltaTime = 0;
		}
	}
}

void HonAController::OnDestroy()
{

}

// trigger event
void HonAController::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
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
		// other4
		GameObject* otherGameObject = other->gameObject;
		string honType = otherGameObject->name;

		// collider off
		collider->SetEnabled(false);
		otherGameObject->GetComponent<CircleCollider>()->SetEnabled(false);
		
		if (honType == "HonA")
		{
			// controller
			HonAController* otherController = otherGameObject->GetComponent<HonAController>();

			// Size를 기준으로 합체 주체 결정
			if (size >= otherController->GetSize())
			{
				if(!otherGameObject->IsDestroyed()) otherGameObject->Destroy();
				SetSize(size * 1.5);
				SetDirection(Vector2::zero);
			}
			else
			{
				otherController->SetSize(otherController->GetSize() * 1.5);
				otherController->SetDirection(Vector2::zero);
				if (!this->gameObject->IsDestroyed()) this->gameObject->Destroy();
			}
		}
		else if (honType == "HonB")
		{

		}
		else if (honType == "HonC")
		{

		}
		else if (honType == "HonD")
		{

		}
	}
}

void HonAController::OnTriggerStay(ICollider* other, const ContactInfo& contact)
{

}

void HonAController::OnTriggerExit(ICollider* other, const ContactInfo& contact)
{

}