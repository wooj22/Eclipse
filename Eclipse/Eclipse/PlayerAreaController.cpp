#include "PlayerAreaController.h"
#include "HonController.h"
#include "PlayerFSM.h"

#include "../Direct2D_EngineLib/CircleCollider.h"

void PlayerAreaController::Awake()
{
	playerFSM = GameObject::Find("Player")->GetComponent<PlayerFSM>();	
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
		other->gameObject->GetComponent<HonController>()->TakeDamageByPlayer(playerFSM->attackDirection);
		OutputDebugStringA("Hon과 충돌 함 \n");
	}
}