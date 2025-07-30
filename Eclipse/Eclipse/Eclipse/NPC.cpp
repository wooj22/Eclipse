#include "NPC.h"

NPC::NPC() : GameObject("NPC") {
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
	rigidbody = AddComponent<Rigidbody>();
	collider = AddComponent<BoxCollider>();
}

void NPC::Awake()
{
	auto npc = ResourceManager::Get().CreateTexture2D("../Resource/mo/NPC.png");
	spriteRenderer->sprite = ResourceManager::Get().CreateSprite(npc, "NPC");
	rigidbody->useGravity = false;
	collider->size = {100, 100};
}

void NPC::SceneStart()
{

}

void NPC::Update()
{
	collider->DebugColliderDraw();
}

void NPC::Destroyed()
{

}
