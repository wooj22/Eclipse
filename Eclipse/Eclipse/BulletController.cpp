#include "BulletController.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Vector2.h"

void BulletController::Awake()
{
	tr = this->gameObject->transform;
	playerTr = GameObject::FindWithTag("Player_Woo")->GetComponent<Transform>();
}

void BulletController::Start()
{

}

void BulletController::Update()
{
	// player dir
	direction = (playerTr->GetWorldPosition() - tr->GetWorldPosition()).Normalized();
	tr->Translate(direction * speed * Time::GetDeltaTime());
}

void BulletController::OnDestroy()
{

}

void BulletController::OnTriggerEnter(ICollider* other)
{
	if (other->gameObject->tag == "Player_Woo")
	{
		// Player TakeDamage
		this->gameObject->Destroy();
	}
}