#include "NPCInteraction.h"
#include "PlayUI.h"
#include "GameManager.h"
#include "../Direct2D_EngineLib/ObjectTable.h"

void NPCInteraction::OnTriggerStay(ICollider* other, const ContactInfo& contact)
{
	if (!GameManager::Get().isWave && !GameManager::Get().isQuest &&  other->gameObject->name == "Player" && 
		ObjectTable::Get().IsValid(other->gameObject) && !interactionImage->IsActive() && !GameManager::Get().g_playUI->ChatActiveCheck())
		interactionImage->SetActive(true);
}

void NPCInteraction::OnTriggerExit(ICollider* other, const ContactInfo& contact)
{
	if (!GameManager::Get().isWave &&other->gameObject->name == "Player" && interactionImage->IsActive() == true)
		interactionImage->SetActive(false);
}

void NPCInteraction::Update()
{
	if (interactionImage->IsActive() == true && (GameManager::Get().g_playUI->ChatActiveCheck() || GameManager::Get().isWave))
		interactionImage->SetActive(false);
}
