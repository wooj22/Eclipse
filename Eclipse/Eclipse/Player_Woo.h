#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/BoxCollider.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "PlayerController_Woo.h"

class Player_Woo : public GameObject
{
public:
	// components
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	Rigidbody* rigidbody;
	BoxCollider* collider;
	PlayerController_Woo* controller;

	Player_Woo()
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		rigidbody = AddComponent<Rigidbody>();
		collider = AddComponent<BoxCollider>();
		controller = AddComponent<PlayerController_Woo>();

		auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Sample/Player_woo.png");
		auto new_sprite = ResourceManager::Get().CreateSprite(texture, "PlayerWoo");
		spriteRenderer->sprite = new_sprite;
		spriteRenderer->layer = 10;

		rigidbody->useGravity = false;
		collider->size = { 50.0f, 80.0f };
	}

	void Update() override
	{
		collider->DebugColliderDraw();
	}
};

