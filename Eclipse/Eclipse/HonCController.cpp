#include "HonCController.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "HonAController.h"
#include "HonBController.h"
#include "HonDController.h"

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
	if (isCollisionMoving)
	{
		// collision move
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
		// collision move start (reset)
		isCollisionMoving = true;
		collisionMovingDelta = 0;

		// direction
		moveDirection = (tr->GetWorldPosition() - playerTr->GetWorldPosition()).Normalized();
	}

	// [hon collision]
	if (other->gameObject->tag == "Hon")
	{
		// other gameobject
		GameObject* otherGameObject = other->gameObject;
		if (otherGameObject->IsDestroyed()) return;
		string honType = otherGameObject->name;

		// 1. 연쇄반응 C-A
		if (honType == "HonA")
		{
			// collision move start (reset)
			isCollisionMoving = true;
			collisionMovingDelta = 0;

			// collider off
			collider->SetEnabled(false);

			// x기준으로 왼쪽애는 left, 오른쪽애는 right로 direction 설정
			float thisX = tr->GetWorldPosition().x;
			float otherX = otherGameObject->transform->GetWorldPosition().x;
			
			if (thisX <= otherX) {
				moveDirection = Vector2::left;
				HonAController* otherController = otherGameObject->GetComponent<HonAController>();
				otherController->SetDirection(Vector2::right);
			}
			else
			{
				moveDirection = Vector2::right;
				HonAController* otherController = otherGameObject->GetComponent<HonAController>();
				otherController->SetDirection(Vector2::left);
			}
		}
		// 2. 연쇄반응 C-C
		else if (honType == "HonC")
		{
			// collision move start (reset)
			isCollisionMoving = true;
			collisionMovingDelta = 0;

			// collider off
			collider->SetEnabled(false);

			// TODO :: 주체 정하기
			// pull position
			Vector2 pullingPos = tr->GetWorldPosition();

			// hon pulling call
			vector<GameObject*> HonList = GameObject::FindAllWithTag("Hon");
			for (GameObject* ob : HonList)
			{
				if (ob->name != "HonC")
				{
					SpriteRenderer* sr = ob->GetComponent<SpriteRenderer>();
					if (Camera::GetMainCamera()->IsInView(ob->transform->GetWorldPosition(), sr->boundSize))
					{
						if (ob->name == "HonA")
							ob->GetComponent<HonAController>()->HonC_PullMe(pullingPos);
						else if(ob->name == "HonB")
							ob->GetComponent<HonBController>()->HonC_PullMe(pullingPos);
						else if (ob->name == "HonD") 
							ob->GetComponent<HonDController>()->HonC_PullMe(pullingPos);
					}
				}
			}

			gameObject->Destroy();
		}

		// collider on
		collider->SetEnabled(true);
	}
}