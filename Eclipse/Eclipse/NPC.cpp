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

	
	interactText = SceneManager::Get().GetCurrentScene()->CreateObject<GameObject>();
	interactText->AddComponent<Transform>();
	interactText->AddComponent<WorldTextRenderer>();
	interactText->transform->SetParent(interactImage->transform);

	interactImage->SetActive(false);
	script->SetNPC(interactImage);

	// animation set
	animatorController = new NpcAnimatorController();
	animator->SetController(animatorController);
}

void NPC::Awake()
{
	transform->SetScale(0.35, 0.35);
	transform->Translate(0, 25);
	spriteRenderer->renderMode = RenderMode::Lit_Glow;
	rigidbody->isKinematic = true;
	collider->isTrigger = true;
	collider->size = { 500, 150 };

	auto npcint = ResourceManager::Get().CreateTexture2D("../Resource/mo/F.png");
	interactImage->GetComponent<SpriteRenderer>()->sprite = ResourceManager::Get().CreateSprite(npcint, "F");
	interactImage->transform->SetParent(transform);
	interactImage->transform->SetPosition(-80, 370);
	interactImage->transform->SetScale(2, 2);

	interactText->transform->SetPosition(140, 5);
	interactText->GetComponent<WorldTextRenderer>()->layer = 2;
	interactText->GetComponent<WorldTextRenderer>()->SetLayout(200, 50);
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
