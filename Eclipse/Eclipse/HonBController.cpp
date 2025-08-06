#include "HonBController.h"
#include "HonB.h"
#include "HonAController.h"
#include "BossController.h"

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
	if (isAbsorption) return;

	if (isPullMoving)
	{
		// pulling move
		pullMovingDelta += Time::GetDeltaTime();
		tr->Translate(pullDirection * collisionSpeed * 1.2f * Time::GetDeltaTime());

		// end pulling
		if (pullMovingDelta >= pullMovingTime) {
			isPullMoving = false;
			pullMovingDelta = 0;

			isCollisionMoving = false;
			collisionMovingDelta = 0;
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
		// score
		if (hp==1) GameManager::Get().honCount++;

		// collision acttion
		CollisionStart();
		moveDirection = (tr->GetWorldPosition() - playerTr->GetWorldPosition()).Normalized();
		TakeDamage();
	}

	// [endline collision]
	if (other->gameObject->tag == "EndLine")
	{
		gameObject->Destroy();
	}

	// [boss collision]
	if (other->gameObject->tag == "Boss")
	{
		other->gameObject->GetComponent<BossController>()->TakeDamage(1);
		gameObject->Destroy();
	}

	// [hon collision]
	if (other->gameObject->tag == "Hon")
	{
		if (gameObject->IsDestroyed()) return;

		// other
		GameObject* otherGameObject = other->gameObject;
		if (otherGameObject->IsDestroyed()) return;

		// collision acttion
		// ¿¬¼â¹ÝÀÀ B-B
		if (otherGameObject->name == "HonB")
		{
			// hp check
			TakeDamage();
			HonBController* otherController = otherGameObject->GetComponent<HonBController>();
			otherController->TakeDamage();
			if (gameObject->IsDestroyed() || otherGameObject->IsDestroyed()) return;

			// position
			std::vector<Vector2> offsets = {
				Vector2(-100, 100),
				Vector2(-100, -100),
				Vector2(100, -100),
				Vector2(100, 100)
			};

			// new HonB
			for (const Vector2& offset : offsets)
			{
				GameObject* newHonB = Instantiate<HonB>(tr->GetWorldPosition() + offset);
				HonBController* controller = newHonB->GetComponent<HonBController>();
				if (controller)
				{
					controller->SetSize(size * 0.7f);
					controller->SetDescentSpeed(descentSpeed * 1.2f);
					controller->SetHp(1);
				}
			}

			// old HonB
			otherGameObject->Destroy();
			gameObject->Destroy();
		}
	}
}