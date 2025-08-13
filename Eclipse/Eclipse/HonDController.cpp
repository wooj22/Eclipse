#include "HonDController.h"
#include "HonAController.h"
#include "HonBController.h"
#include "HonCController.h"
#include "BossController.h"

/*------------- Cycle  -------------*/
void HonDController::Awake()
{
	tr = gameObject->transform;
	sr = gameObject->GetComponent<SpriteRenderer>();
	collider = gameObject->GetComponent<CircleCollider>();
	audioSource = gameObject->GetComponent<AudioSource>();
	playerTr = GameObject::Find("Player")->GetComponent<Transform>();

	sr->renderMode = RenderMode::Lit_ColorTint;
	sr->SetGlowAmmount(10);
	
	SetSize(size);
}

void HonDController::Update()
{
	// sound delay destroy
	if (destroyPending)
	{
		if (!audioSource->IsPlaying()) gameObject->Destroy();
		else
		{
			// opacity ¿¬Ãâ
			OpacityDirecting();
			return;
		}
	}

	// moving
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

	// [endline collision]
	if (other->gameObject->tag == "EndLine")
	{
		// wave3 quest
		GameManager::Get().ChangeQuestCount(3);

		gameObject->Destroy();
	}

	// [mapborder collision]
	if (other->gameObject->name == "MapBorder")
	{
		CollisionStart();
		moveDirection = contact.normal;
	}

	// [boss collision]
	if (other->gameObject->tag == "Boss")
	{
		Instantiate<BossHitEffect>(tr->GetWorldPosition());
		other->gameObject->GetComponent<BossController>()->TakeDamage(1);
		gameObject->Destroy();
	}

	// [hon collision]
	if (other->gameObject->tag == "Hon")
	{
		if (gameObject->IsDestroyed() || destroyPending) return;

		// other
		GameObject* otherGameObject = other->gameObject;
		if (otherGameObject->IsDestroyed()) return;
		HonController* otherController = otherGameObject->GetComponent<HonController>();
		if (otherController->destroyPending) return;
		string honType = otherGameObject->name;

		// collision acttion
		otherController->isCollisionD = true;
		otherController->TakeDamage(5);
		TakeDamage(1);
	}
}


// Player Attack
void HonDController::TakeDamageByPlayer(Vector2 dir)
{
	TakeDamage(2);
}