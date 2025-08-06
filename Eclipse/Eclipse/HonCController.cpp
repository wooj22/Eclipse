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
	collider = gameObject->GetComponent<CircleCollider>();
	playerTr = GameObject::Find("Player")->GetComponent<Transform>();
}

void HonCController::Start()
{
	tr->SetScale(size, size);
}

void HonCController::Update()
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
	if (other->gameObject->name == "PlayerAttackArea")
	{
		// score
		if (hp == 1) GameManager::Get().honCount++;

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
		case HonType::A:		// 연쇄 반응 C-A
		{
			// hp cheak
			TakeDamage();
			otherController->TakeDamage();
			if (gameObject->IsDestroyed() || otherGameObject->IsDestroyed()) return;

			// score
			GameManager::Get().honCount++;

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
			break;
		}
		case HonType::B:		// 연쇄 반응 C-B
		{
			// score
			GameManager::Get().honCount++;

			// collision action
			TakeDamage();
			otherController->TakeDamage();
			break;
		}
		case HonType::C:		// 연쇄 반응 C-C
		{
			// score
			GameManager::Get().honCount++;

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

			if (!other->gameObject->IsDestroyed()) other->gameObject->Destroy();
			gameObject->Destroy();
			break;
		}
		default:
			break;
		}
	}
}