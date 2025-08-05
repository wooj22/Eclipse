#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"

#include "PlayerAreaController.h";

class PlayerAttackArea : public GameObject
{
	// [ components ]
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	// Rigidbody* rigidbody;
	CircleCollider* collider;

	PlayerAreaController* controller;

public:
	PlayerAttackArea() : GameObject("PlayerAttackArea", "PlayerAttackArea")
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		collider = AddComponent<CircleCollider>();
		controller = AddComponent<PlayerAreaController>();

		spriteRenderer->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Attack_Effect.png"), "Attack_Effect");
		spriteRenderer->layer = 2;

	}
	~PlayerAttackArea() override
	{

	}

	void Awake() override
	{
		transform->SetPosition(0.0f, 30.0f);
		transform->SetRotation(0.0f);
		transform->SetScale(3.0f, 3.0f);

		collider->offset = { 0.0f, 20.0f };
		collider->radius = 60.0f;
		collider->isTrigger = true;
	}

	void Update() override
	{
		// AABB ¿µ¿ª 
		collider->DebugColliderDraw();
	}
};