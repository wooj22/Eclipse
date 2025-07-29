#include "Player_Test.h"

Player_Test::Player_Test() : GameObject("Player") {
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
	rigidbody = AddComponent<Rigidbody>();
	collider = AddComponent<BoxCollider>();
	scirpt = AddComponent<PlayerMove>();
}

void Player_Test::Awake()
{
	auto player = ResourceManager::Get().CreateTexture2D("../Resource/mo/Player.png");
	spriteRenderer->sprite = ResourceManager::Get().CreateSprite(player, "Player");
	rigidbody->useGravity = false;
	collider->size = { 100,100 };
}

void Player_Test::SceneStart()
{
	
}

void Player_Test::Update()
{
	collider->DebugColliderDraw();
}

void Player_Test::Destroyed()
{

}