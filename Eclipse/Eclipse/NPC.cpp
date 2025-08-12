#include "NPC.h"

NPC::NPC() : GameObject("NPC") {
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
	rigidbody = AddComponent<Rigidbody>();
	collider = AddComponent<BoxCollider>();
	animator = AddComponent<Animator>();
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

	// animation set
	animatorController = new NpcAnimatorController();
	animator->SetController(animatorController);
}

void NPC::Awake()
{
	transform->SetScale(1.5, 1.5);
	transform->Translate(0, 25);
	spriteRenderer->renderMode = RenderMode::Lit_Glow;
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
