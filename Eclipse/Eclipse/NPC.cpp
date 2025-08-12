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

	
	interactText = SceneManager::Get().GetCurrentScene()->CreateObject<GameObject>();
	interactText->AddComponent<Transform>();
	interactText->AddComponent<WorldTextRenderer>();
	interactText->transform->SetParent(interactImage->transform);

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

	auto npcint = ResourceManager::Get().CreateTexture2D("../Resource/mo/F.png");
	interactImage->GetComponent<SpriteRenderer>()->sprite = ResourceManager::Get().CreateSprite(npcint, "F");
	interactImage->transform->SetParent(transform);
	interactImage->transform->SetPosition(0, 100);
	interactImage->transform->SetScale(0.5, 0.5);

	interactText->transform->SetPosition(150, 0);
	interactText->GetComponent<WorldTextRenderer>()->SetLayout(200,50);
	interactText->GetComponent<WorldTextRenderer>()->SetText(L"상호 작용");
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
