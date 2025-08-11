#include "HonAController.h"
#include "HonBController.h"
#include "HonCController.h"
#include "HonDController.h"
#include "BossController.h"

/*------------- Cycle  -------------*/
void HonAController::Awake()
{
	tr = gameObject->transform;
	sr = gameObject->GetComponent<SpriteRenderer>();
	collider = gameObject->GetComponent<CircleCollider>();
	audioSource = gameObject->GetComponent<AudioSource>();
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
	if (other->gameObject->name == "Player")
	{
		MovementFSM* playerFSM = other->gameObject->GetComponent<PlayerFSM>()->GetMovementFSM();
		if (!playerFSM->IsInState<Attack_State>())
		{
			other->gameObject->GetComponent<PlayerFSM>()->SetSpeedDownRate(palyer_deceleration);
		}
	}

	// [endline collision]
	if (other->gameObject->tag == "EndLine")
	{
		// score
		GameManager::Get().ChangeHonCount(-1);
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
		{
			// hp cheak
			TakeDamage(1);
			otherController->TakeDamage(1);
			if (gameObject->IsDestroyed() || otherGameObject->IsDestroyed()) return;

			// wave2 quest
			GameManager::Get().ChangeQuestCount(2);

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
				otherController->is2A = true;
				if (!this->gameObject->IsDestroyed()) this->gameObject->Destroy();
			}

			// sound
			audioSource->SetClip(SFX_HonMerge);
			audioSource->PlayOneShot();

			break;
		}

		case HonType::B:		// 연쇄반응 A-B
		{
			// score
			GameManager::Get().ChangeHonCount(1);

			// hp cheak
			TakeDamage(1);
			otherController->TakeDamage(1);
			if (gameObject->IsDestroyed() || otherGameObject->IsDestroyed()) return;

			// wave2 quest
			GameManager::Get().ChangeQuestCount(2);

			// collision move start
			moveDirection = (tr->GetWorldPosition() - otherGameObject->transform->GetWorldPosition()).Normalized();
			otherController->SetDirection((otherGameObject->transform->GetWorldPosition() - tr->GetWorldPosition()).Normalized());
			CollisionStart();
			otherController->CollisionStart();

			// sound
			audioSource->SetClip(SFX_HonCollision);
			audioSource->PlayOneShot();

			break;
		}

		default:
			break;
		}
	}
}

// Player Attack
void HonAController::TakeDamageByPlayer()
{
	// collision acttion
	CollisionStart();
	moveDirection = (tr->GetWorldPosition() - playerTr->GetWorldPosition()).Normalized();
	TakeDamage(1);
}