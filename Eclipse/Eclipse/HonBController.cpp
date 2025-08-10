#include "HonBController.h"
#include "HonB.h"
#include "HonAController.h"
#include "BossController.h"

/*------------- Cycle  -------------*/
void HonBController::Awake()
{
	tr = gameObject->transform;
	collider = gameObject->GetComponent<CircleCollider>();
	audioSource = gameObject->GetComponent<AudioSource>();
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
		case HonType::B :		// 연쇄 반응 B-B
		{
			// hp check
			TakeDamage(1);
			otherController->TakeDamage(1);
			if (gameObject->IsDestroyed() || otherGameObject->IsDestroyed()) return;

			// wave2 quest
			GameManager::Get().ChangeQuestCount(2);

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

			// sound
			audioSource->SetClip(SFX_Division);
			audioSource->PlayOneShot();

			// old HonB
			otherGameObject->Destroy();
			gameObject->Destroy();
		}
		case HonType::C:		// 연쇄 반응 B-C (C쪽에서 처리해서 자기 보정만 담당)
		{
			honBC_dir = other->gameObject->transform->GetWorldPosition() - tr->GetWorldPosition();
		}
		}
	}
}

// Hon B-C 겹침 불가
void HonBController::OnTriggerStay(ICollider* other, const ContactInfo& contact)
{
	if (other->gameObject->name == "HonC")
	{
		honBC_dir = honBC_dir.Normalized();
		tr->Translate(honBC_dir * collisionSpeed * 1.3 * Time::GetDeltaTime());
	}
}


// Player Attack
void HonBController::TakeDamageByPlayer()
{
	// collision acttion
	CollisionStart();
	moveDirection = (tr->GetWorldPosition() - playerTr->GetWorldPosition()).Normalized();
	TakeDamage(1);
}