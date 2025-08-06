#include "HonDController.h"
#include "HonAController.h"
#include "HonBController.h"
#include "HonCController.h"
#include "BossController.h"

/*------------- Cycle  -------------*/
void HonDController::Awake()
{
	tr = gameObject->transform;
	collider = gameObject->GetComponent<CircleCollider>();
	playerTr = GameObject::Find("Player")->GetComponent<Transform>();

	SetSize(size);
}

void HonDController::Update()
{
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


/*------------- Trigger Event -------------*/
void HonDController::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
{
	// [player collision]
	if (other->gameObject->name == "Player")
	{
		MovementFSM* playerFSM = other->gameObject->GetComponent<PlayerFSM>()->GetMovementFSM();
		if (!playerFSM->IsInState<Attack_State>())
		{
			other->gameObject->GetComponent<PlayerFSM>()->SetSpeedDownRate(palyer_deceleration);
		}
	}

	// [player attack collision]
	if (other->gameObject->name == "PlayerAttackArea")
	{
		//// score
		//GameManager::Get().honCount--;

		//gameObject->Destroy();
	}

	// [endline collision]
	if (other->gameObject->tag == "EndLine")
	{
		// wave3 quest
		GameManager::Get().lunaKillCount++;

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
		string honType = otherGameObject->name;

		// score
		GameManager::Get().honCount--;

		// collision acttion
		if(honType == "HonD") otherGameObject->Destroy();
		else otherGameObject->GetComponent<HonController>()->TakeDamage();
		this->gameObject->Destroy();
	}
}


// Player Attack
void HonDController::TakeDamageByPlayer()
{
	// score
	GameManager::Get().honCount--;

	gameObject->Destroy();
}