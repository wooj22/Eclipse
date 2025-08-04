#include "NPC.h"

NPC::NPC() : GameObject("NPC") {
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
	rigidbody = AddComponent<Rigidbody>();
	collider = AddComponent<BoxCollider>();
	script = AddComponent<NPCInteraction>();

	interactImage = SceneManager::Get().GetCurrentScene()->CreateObject<GameObject>();
	interactImage->AddComponent<Transform>();
	interactImage->AddComponent<SpriteRenderer>();
	auto npcint = ResourceManager::Get().CreateTexture2D("../Resource/mo/NPCIntercetion.png");
	interactImage->GetComponent<SpriteRenderer>()->sprite = ResourceManager::Get().CreateSprite(npcint, "NPCIntercetion");
	interactImage->transform->SetParent(transform);
	interactImage->transform->SetPosition(0, 100);
	interactImage->transform->SetScale(0.2f, 0.2f);
	interactImage->SetActive(false);
	script->SetNPC(interactImage);
}

void NPC::Awake()
{
	auto npc = ResourceManager::Get().CreateTexture2D("../Resource/mo/NPC.png");
	spriteRenderer->sprite = ResourceManager::Get().CreateSprite(npc, "NPC");
	rigidbody->isKinematic = true;
	collider->isTrigger = true;
	collider->size = {100, 100};
}

void NPC::SceneStart()
{

}

void NPC::Update()
{
	//collider->DebugColliderDraw();
}

void NPC::Destroyed()
{

}
