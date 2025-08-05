#include "HonAController.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Time.h"
#include "HonBController.h"

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

		// 1. 연쇄반응 A-A
		if (honType == "HonA")
		{
			// hp cheak
			
			// collision move start (reset)
			CollisionStart();

			// controller
			HonAController* otherController = otherGameObject->GetComponent<HonAController>();

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
				otherController->SetHp(otherController->GetHp());
				if (!this->gameObject->IsDestroyed()) this->gameObject->Destroy();
			}
		}
		// 2. 연쇄반응 A-B
		else if (honType == "HonB")
		{
			// collision move start (reset)
			CollisionStart();

			HonBController* otherController = otherGameObject->GetComponent<HonBController>();
			moveDirection = (tr->GetWorldPosition() - otherGameObject->transform->GetWorldPosition()).Normalized();
		
			hp--;
		}
		else if (honType == "HonC")
		{
			// collision move start (reset)
			CollisionStart();

			// 로직은 C쪽에서 처리
			hp--;
		}
	}

	// HP Cheak
	if (hp <= 0) gameObject->Destroy();
}


/*------------- Functions -------------*/
// C-C
void HonAController::HonC_PullMe(Vector2 pos)
{
	pullDirection = (pos - tr->GetWorldPosition()).Normalized();
	isPullMoving = true;
}