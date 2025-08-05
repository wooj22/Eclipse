#include "HonDController.h"
#include "HonAController.h"
#include "HonBController.h"
#include "HonCController.h"

/*------------- Cycle  -------------*/
void HonDController::Awake()
{
	tr = gameObject->transform;
	collider = gameObject->GetComponent<CircleCollider>();
	playerTr = GameObject::Find("Player")->GetComponent<Transform>();
}

void HonDController::Update()
{
	if (isPullMoving)
	{
		// pulling move
		pullMovingDelta += Time::GetDeltaTime();
		tr->Translate(pullDirection * collisionSpeed * 2 * Time::GetDeltaTime());

		// end pulling
		if (pullMovingDelta >= pullMovingTime) {
			isPullMoving = false;
			pullMovingDelta = 0;
		}
	}
	else if (isCollisionMoving)
	{
		// collision move
		collisionMovingDelta += Time::GetDeltaTime();
		tr->Translate(moveDirection * collisionSpeed * Time::GetDeltaTime());

		// end collidion moving
		if (collisionMovingDelta >= collisionMovingTime)
		{
			isCollisionMoving = false;
			collisionMovingDelta = 0;
		}
	}
	else
	{
		// descent move
		tr->Translate(descentDirection * descentSpeed * Time::GetDeltaTime());
	}
}


/*------------- Trigger Event -------------*/
void HonDController::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
{
	// [player collision]
	if (other->gameObject->name == "PlayerAttackArea")
	{
		CollisionStart();
		moveDirection = (tr->GetWorldPosition() - playerTr->GetWorldPosition()).Normalized();
	}

	// [hon collision]
	if (other->gameObject->tag == "Hon")
	{
		if (gameObject->IsDestroyed()) return;

		// other gameobject
		GameObject* otherGameObject = other->gameObject;
		if (otherGameObject->IsDestroyed()) return;
		string honType = otherGameObject->name;

		// destroy
		if (honType == "HonA")
		{
			otherGameObject->GetComponent<HonAController>()->TakeDamage();
		}
		else if(honType == "HonB")
		{
			otherGameObject->GetComponent<HonBController>()->TakeDamage();
		}
		else if (honType == "HonC")
		{
			otherGameObject->GetComponent<HonCController>()->TakeDamage();
		}
		else if(honType == "HonD")
		{
			otherGameObject->Destroy();
		}
		this->gameObject->Destroy();
	}
}