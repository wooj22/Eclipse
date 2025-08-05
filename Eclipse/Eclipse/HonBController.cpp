#include "HonBController.h"
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
		CollisionStart();
		moveDirection = (tr->GetWorldPosition() - playerTr->GetWorldPosition()).Normalized();
		TakeDamage();
	}

	// [hon collision]
	if (other->gameObject->tag == "Hon")
	{
		if (gameObject->IsDestroyed()) return;

		// other
		GameObject* otherGameObject = other->gameObject;
		if (otherGameObject->IsDestroyed()) return;
		string honType = otherGameObject->name;

		// 연쇄반응 B-B
		if (honType == "HonB")
		{
			// hp cheak
			TakeDamage();
			HonBController* otherController = otherGameObject->GetComponent<HonBController>();
			otherController->TakeDamage();
			if (gameObject->IsDestroyed() || otherGameObject->IsDestroyed()) return;

			// collision move start
			// 일단 하드코딩!
			GameObject* newHonB1 = Instantiate<HonB>(tr->GetWorldPosition() + Vector2(-100, 100));
			HonBController* controller = newHonB1->GetComponent<HonBController>();
			controller->SetSize(size * 0.7);
			controller->SetDescentSpeed(descentSpeed * 1.2);
			controller->SetHp(1);

			GameObject* newHonB2 = Instantiate<HonB>(tr->GetWorldPosition() + Vector2(-100, -100));
			HonBController* controller1 = newHonB2->GetComponent<HonBController>();
			controller1->SetSize(size * 0.7);
			controller1->SetDescentSpeed(descentSpeed * 1.2);
			controller1->SetHp(1);

			GameObject* newHonB3 = Instantiate<HonB>(tr->GetWorldPosition() + Vector2(100, -100));
			HonBController* controller2 = newHonB3->GetComponent<HonBController>();
			controller2->SetSize(size * 0.7);
			controller2->SetDescentSpeed(descentSpeed * 1.2);
			controller2->SetHp(1);

			GameObject* newHonB4 = Instantiate<HonB>(tr->GetWorldPosition() + Vector2(100, 100));
			HonBController* controller3 = newHonB4->GetComponent<HonBController>();
			controller3->SetSize(size * 0.7);
			controller3->SetDescentSpeed(descentSpeed * 1.2);
			controller3->SetHp(1);

			otherGameObject->Destroy();
			gameObject->Destroy();
		}
	}
}