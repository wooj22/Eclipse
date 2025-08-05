#include "HonAController.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Time.h"
#include "HonBController.h"
#include "HonCController.h"

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

void HonAController::OnDestroy()
{

}


/*------------- Trigger Event -------------*/
void HonAController::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
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

		// other gameobject
		GameObject* otherGameObject = other->gameObject;
		if (otherGameObject->IsDestroyed()) return;
		string honType = otherGameObject->name;

		// 연쇄반응 A-A
		if (honType == "HonA")
		{
			// hp cheak
			TakeDamage();
			HonAController* otherController = otherGameObject->GetComponent<HonAController>();
			otherController->TakeDamage();
			if (gameObject->IsDestroyed() || otherGameObject->IsDestroyed()) return;
			
			// collision move start
			// Size를 기준으로 합체 주체 결정
			if (size >= otherController->GetSize())
			{
				if(!otherGameObject->IsDestroyed()) otherGameObject->Destroy();
				SetSize(size * 1.4);
				CollisionEnd();
				SetDescentSpeed(descentSpeed * 0.6);
				SetHp(1);
			}
			else
			{
				otherController->SetSize(otherController->GetSize() * 1.5);
				otherController->CollisionEnd();
				otherController->SetDescentSpeed(otherController->GetSDescentpeed() * 0.6);
				otherController->SetHp(1);
				if (!this->gameObject->IsDestroyed()) this->gameObject->Destroy();
			}
		}
		// 연쇄반응 A-B
		else if (honType == "HonB")
		{
			// hp cheak
			TakeDamage();
			HonBController* otherController = otherGameObject->GetComponent<HonBController>();
			otherController->TakeDamage();
			if (gameObject->IsDestroyed() || otherGameObject->IsDestroyed()) return;

			// collision move start
			moveDirection = (tr->GetWorldPosition() - otherGameObject->transform->GetWorldPosition()).Normalized();
			otherController->SetDirection((otherGameObject->transform->GetWorldPosition() - tr->GetWorldPosition()).Normalized());
			CollisionStart();
			otherController->CollisionStart();
		}
	}
}


/*------------- Functions -------------*/
// C-C
void HonAController::HonC_PullMe(Vector2 pos)
{
	pullDirection = (pos - tr->GetWorldPosition()).Normalized();
	isPullMoving = true;
}