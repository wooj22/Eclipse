#include "HonCController.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Time.h"
#include "HonAController.h"
#include "HonBController.h"

// script component cycle
void HonCController::Awake()
{
	tr = gameObject->transform;
	collider = gameObject->GetComponent<CircleCollider>();

	playerTr = GameObject::Find("Player")->GetComponent<Transform>();
}

void HonCController::Start()
{
	tr->SetScale(size, size);
}

void HonCController::Update()
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

void HonCController::OnDestroy()
{

}

// trigger event
void HonCController::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
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
			// x기준으로 왼쪽애는 left, 오른쪽애는 right로 direction 설정
			float thisX = tr->GetWorldPosition().x;
			float otherX = otherGameObject->transform->GetWorldPosition().x;
			
			if (thisX <= otherX) {
				direction = Vector2::left;
				HonAController* otherController = otherGameObject->GetComponent<HonAController>();
				otherController->SetDirection(Vector2::right);
			}
			else
			{
				direction = Vector2::right;
				HonAController* otherController = otherGameObject->GetComponent<HonAController>();
				otherController->SetDirection(Vector2::left);
			}
		}
		else if (honType == "HonC")
		{

		}

		// collider on
		collider->SetEnabled(true);
	}
}