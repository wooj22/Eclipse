#include "BulletController.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Vector2.h"

void BulletController::Awake()
{
	tr = this->gameObject->transform;
}

void BulletController::Start()
{

}

void BulletController::Update()
{
	tr->Translate(direction * speed * Time::GetDeltaTime());
}

void BulletController::OnDestroy()
{

}

void BulletController::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
{
	if (other->gameObject->tag == "Player_Woo")
	{
		// Player TakeDamage
		this->gameObject->Destroy();
	}
}