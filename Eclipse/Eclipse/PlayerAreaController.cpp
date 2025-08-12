#include "PlayerAreaController.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "HonController.h"

void PlayerAreaController::Awake()
{

}

void PlayerAreaController::Start()
{

}

void PlayerAreaController::Update()
{

}

void PlayerAreaController::OnDestroy()
{

}


void PlayerAreaController::OnTriggerEnter(ICollider* other, const ContactInfo& contact) 
{
	if (other->gameObject->tag == "Hon")
	{
		other->gameObject->GetComponent<HonController>()->TakeDamageByPlayer(gameObject->GetComponent<PlayerFSM>()->attackDirection);
		OutputDebugStringA("Hon과 충돌 함 \n");
	}
}