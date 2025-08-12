#include "HonCController.h"
#include "HonAController.h"
#include "HonBController.h"
#include "HonDController.h"
#include "BossController.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"

/*------------- Cycle  -------------*/
void HonCController::Awake()
{
	tr = gameObject->transform;
	sr = gameObject->GetComponent<SpriteRenderer>();
	collider = gameObject->GetComponent<CircleCollider>();
	audioSource = gameObject->GetComponent<AudioSource>();
	playerTr = GameObject::Find("Player")->GetComponent<Transform>();

	sr->renderMode = RenderMode::Lit_ColorTint;
	sr->SetGlowAmmount(10);
}

void HonCController::Start()
{
	tr->SetScale(size, size);
}

void HonCController::Update()
{
	// sound delay destroy
	if (destroyPending)
	{
		if (!audioSource->IsPlaying()) gameObject->Destroy();
		return;
	}

	// moving
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
		// end collidion moving
		collisionMovingDelta += Time::GetDeltaTime();
		tr->Translate(moveDirection * collisionSpeed * Time::GetDeltaTime());

		// move end
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

void HonCController::OnDestroy()
{

}


/*------------- Trigger Event -------------*/
void HonCController::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
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
		GameManager::Get().ChangeHonCount(-10);
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
		HonType honType = otherController->honType;

		// collision acttion
		switch (honType)
		{
		case HonType::A:		// 연쇄 반응 C-A
		{
			// score
			GameManager::Get().ChangeHonCount(10);

			// hp cheak
			TakeDamage(1);
			otherController->TakeDamage(1);
			if (destroyPending || otherController->destroyPending) return;

			// wave2 quest
			GameManager::Get().ChangeQuestCount(2);

			// collision move start (reset)
			// x기준으로 왼쪽애는 left, 오른쪽애는 right로 direction 설정
			float thisX = tr->GetWorldPosition().x;
			float otherX = otherGameObject->transform->GetWorldPosition().x;

			if (thisX <= otherX) {
				moveDirection = Vector2::left;
				otherController->SetDirection(Vector2::right);
			}
			else
			{
				moveDirection = Vector2::right;
				otherController->SetDirection(Vector2::left);
			}

			otherController->CollisionStart();
			CollisionStart();

			// sound
			audioSource->SetClip(SFX_HonCollision);
			audioSource->PlayOneShot();

			break;
		}
		case HonType::B:		// 연쇄 반응 C-B
		{
			// score
			GameManager::Get().ChangeHonCount(10);

			// wave2 quest
			GameManager::Get().ChangeQuestCount(2);

			// collision action
			// hon b 겹침 x
			honCB_dir = other->gameObject->transform->GetWorldPosition() - tr->GetWorldPosition();

			// sound
			audioSource->SetClip(SFX_HonCollision);
			audioSource->PlayOneShot();

			TakeDamage(1);
			otherController->TakeDamage(1);
			break;
		}
		case HonType::C:		// 연쇄 반응 C-C
		{
			// hp check
			TakeDamage(1);
			otherController->TakeDamage(1);
			if (destroyPending || otherController->destroyPending) return;

			// wave2 quest
			GameManager::Get().ChangeQuestCount(2);

			// collision action
			// pull position
			Vector2 pullingPos = tr->GetWorldPosition();

			// hon pulling call
			vector<GameObject*> HonList = GameObject::FindAllWithTag("Hon");
			for (GameObject* ob : HonList)
			{
				SpriteRenderer* sr = ob->GetComponent<SpriteRenderer>();
				if (Camera::GetMainCamera()->IsInView(ob->transform->GetWorldPosition(), sr->boundSize))
				{
					ob->GetComponent<HonController>()->HonC_PullMe(pullingPos);
				}
			}

			// sound
			audioSource->SetClip(SFX_HonCollision);
			audioSource->PlayOneShot();

			if (!other->gameObject->IsDestroyed()) otherController->TakeDamage(3);
			TakeDamage(3);
			break;
		}
		default:
			break;
		}
	}
}

// Hon B-C 겹침 불가
void HonCController::OnTriggerStay(ICollider* other, const ContactInfo& contact)
{
	// hon b랑 겹쳐있으면 관통시키기
	if (other->gameObject->name == "HonB")
	{
		honCB_dir = honCB_dir.Normalized();
		tr->Translate(honCB_dir * collisionSpeed * 1.3 * Time::GetDeltaTime());
	}
}


// Player Attack
void HonCController::TakeDamageByPlayer(Vector2 dir)
{
	// collision acttion
	CollisionStart();
	moveDirection = dir;
	TakeDamage(2);
}