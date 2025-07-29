#include "Player.h"

Player::Player() : GameObject("Player") {
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
	rigidbody = AddComponent<Rigidbody>();
	collider = AddComponent<BoxCollider>();
	scirpt = AddComponent<PlayerMove>();
}

void Player::Awake()
{
	auto player = ResourceManager::Get().CreateTexture2D("../Resource/Player.png");
	spriteRenderer->sprite = ResourceManager::Get().CreateSprite(player, "Player");
	rigidbody->useGravity = false;
}

void Player::SceneStart()
{
	
}

void Player::Update()
{
	collider->DebugColliderDraw();
}

void Player::Destroyed()
{

}