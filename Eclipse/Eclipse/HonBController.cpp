#include "HonBController.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Time.h"
#include "HonB.h"
#include "HonAController.h"

/*------------- Cycle  -------------*/
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

void HonBController::OnDestroy()
{

}


/*------------- Trigger Event -------------*/
void HonBController::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
{
	// [player collision]
	if (other->gameObject->name == "PlayerAttackArea")
	{
		// collision move start (reset)
		CollisionStart();

		// direction
		moveDirection = (tr->GetWorldPosition() - playerTr->GetWorldPosition()).Normalized();

		// hp
		TakeDamage();
	}

	// [hon collision]
	if (other->gameObject->tag == "Hon")
	{
		// other gameobject
		GameObject* otherGameObject = other->gameObject;
		if (otherGameObject->IsDestroyed()) return;
		string honType = otherGameObject->name;

		// 1. ø¨º‚π›¿¿ B-A
		if (honType == "HonA")
		{
			// collision move start (reset)
			CollisionStart();

			HonAController* otherController = otherGameObject->GetComponent<HonAController>();
			moveDirection = (tr->GetWorldPosition() - otherGameObject->transform->GetWorldPosition()).Normalized();
		
			hp--;
		}
		// 2. ø¨º‚π›¿¿ B-B
		else if (honType == "HonB")
		{
			// collision move start (reset)
			CollisionStart();

			SetSize(size * 0.7);
			SetDescentSpeed(descentSpeed * 1.2);
			SetDirection(Vector2::down);
			hp--;
			if (hp <= 0) gameObject->Destroy();
			else
			{
				GameObject* newHonB = Instantiate<HonB>(tr->GetWorldPosition() + Vector2(150, 150));
				HonBController* controller = newHonB->GetComponent<HonBController>();
				controller->SetSize(size);
				controller->SetDescentSpeed(descentSpeed * 1.2);
				controller->SetHp(hp);
			}
		}
	}

	// HP Cheak
	if (hp <= 0) gameObject->Destroy();
}


/*------------- Functions -------------*/
// C-C
void HonBController::HonC_PullMe(Vector2 pos)
{
	pullDirection = (pos - tr->GetWorldPosition()).Normalized();
	isPullMoving = true;
}