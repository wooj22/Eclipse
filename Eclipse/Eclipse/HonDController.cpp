#include "HonDController.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Time.h"

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

		// move end
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
		// collision move start (reset)
		isCollisionMoving = true;
		collisionMovingDelta = 0;

		// direction
		moveDirection = (tr->GetWorldPosition() - playerTr->GetWorldPosition()).Normalized();

		// hp
		hp--;
	}

	// [hon collision]
	if (other->gameObject->tag == "Hon")
	{
		// other gameobject
		GameObject* otherGameObject = other->gameObject;
		if (otherGameObject->IsDestroyed()) return;
		string honType = otherGameObject->name;

		// collision move start (reset)
		isCollisionMoving = true;
		collisionMovingDelta = 0;

		// destroy
		otherGameObject->Destroy();
		this->gameObject->Destroy();
	}

	// HP Cheak
	if (hp <= 0) gameObject->Destroy();
}

/*------------- Functions -------------*/
void HonDController::HonC_PullMe(Vector2 pos)
{
	pullDirection = (pos - tr->GetWorldPosition()).Normalized();
	isPullMoving = true;
}