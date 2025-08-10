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
	audioSource = gameObject->GetComponent<AudioSource>();
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
		case HonType::A:		// ���� ���� C-A
		{
			// score
			GameManager::Get().ChangeHonCount(1);

			// hp cheak
			TakeDamage(1);
			otherController->TakeDamage(1);
			if (gameObject->IsDestroyed() || otherGameObject->IsDestroyed()) return;

			// wave2 quest
			GameManager::Get().ChangeQuestCount(2);

			// collision move start (reset)
			// x�������� ���ʾִ� left, �����ʾִ� right�� direction ����
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
			audioSource->SetClip(SFX_Collision);
			audioSource->PlayOneShot();

			break;
		}
		case HonType::B:		// ���� ���� C-B
		{
			// score
			GameManager::Get().ChangeHonCount(1);

			// wave2 quest
			GameManager::Get().ChangeQuestCount(2);

			// collision action
			// hon b ��ħ x
			honCB_dir = other->gameObject->transform->GetWorldPosition() - tr->GetWorldPosition();

			// sound
			audioSource->SetClip(SFX_Collision);
			audioSource->PlayOneShot();

			TakeDamage(1);
			otherController->TakeDamage(1);
			break;
		}
		case HonType::C:		// ���� ���� C-C
		{
			// hp check
			TakeDamage(1);
			otherController->TakeDamage(1);
			if (gameObject->IsDestroyed() || otherGameObject->IsDestroyed()) return;

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
			audioSource->SetClip(SFX_Collision);
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

// Hon B-C ��ħ �Ұ�
void HonCController::OnTriggerStay(ICollider* other, const ContactInfo& contact)
{
	// hon b�� ���������� �����Ű��
	if (other->gameObject->name == "HonB")
	{
		honCB_dir = honCB_dir.Normalized();
		tr->Translate(honCB_dir * collisionSpeed * 1.3 * Time::GetDeltaTime());
	}
}


// Player Attack
void HonCController::TakeDamageByPlayer()
{
	// collision acttion
	CollisionStart();
	moveDirection = (tr->GetWorldPosition() - playerTr->GetWorldPosition()).Normalized();
	TakeDamage(1);
}