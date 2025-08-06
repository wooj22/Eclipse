#include "HonAController.h"
#include "HonBController.h"
#include "HonCController.h"
#include "HonDController.h"
#include "BossController.h"

/*------------- Cycle  -------------*/
void HonAController::Awake()
{
	tr = gameObject->transform;
	collider = gameObject->GetComponent<CircleCollider>();
	playerTr = GameObject::Find("Player")->GetComponent<Transform>();
}

void HonAController::Start()
{
	tr->SetScale(size, size);
}

void HonAController::Update()
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

void HonAController::OnDestroy()
{

}


/*------------- Trigger Event -------------*/
void HonAController::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
{
	// [player collision]
	if (other->gameObject->name == "PlayerAttackArea")
	{
		// score
		if (is2A) GameManager::Get().honCount += 3;
		else  GameManager::Get().honCount += 1;

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
		HonController* otherController = otherGameObject->GetComponent<HonController>();
		HonType honType = otherController->honType;

		// collision acttion
		switch (honType)
		{
		case HonType::A:		// 연쇄반응 A-A
			// hp cheak
			TakeDamage();
			otherController->TakeDamage();
			if (gameObject->IsDestroyed() || otherGameObject->IsDestroyed()) return;

			// score
			GameManager::Get().honCount++;

			// collision move start
			// Size를 기준으로 합체 주체 결정
			if (size >= otherController->GetSize())
			{
				if (!otherGameObject->IsDestroyed()) otherGameObject->Destroy();
				SetSize(size * 1.4);
				CollisionEnd();
				SetDescentSpeed(descentSpeed * 0.6);
				SetHp(1);
				is2A = true;
			}
			else
			{
				otherController->SetSize(otherController->GetSize() * 1.5);
				otherController->CollisionEnd();
				otherController->SetDescentSpeed(otherController->GetSDescentpeed() * 0.6);
				otherController->SetHp(1);
				otherGameObject->GetComponent<HonAController>()->is2A = true;
				if (!this->gameObject->IsDestroyed()) this->gameObject->Destroy();
			}

			break;

		case HonType::B:		// 연쇄반응 A-B
			// hp cheak
			TakeDamage();
			otherController->TakeDamage();
			if (gameObject->IsDestroyed() || otherGameObject->IsDestroyed()) return;

			// score
			GameManager::Get().honCount++;

			// collision move start
			moveDirection = (tr->GetWorldPosition() - otherGameObject->transform->GetWorldPosition()).Normalized();
			otherController->SetDirection((otherGameObject->transform->GetWorldPosition() - tr->GetWorldPosition()).Normalized());
			CollisionStart();
			otherController->CollisionStart();
			break;

		default:
			break;
		}
	}
}