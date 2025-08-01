#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"

class PlayerAttackArea : public GameObject
{
	// [ components ]
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	// Rigidbody* rigidbody;
	CircleCollider* collider;

public:
	PlayerAttackArea() : GameObject("PlayerAttackArea", "PlayerAttackArea")
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		collider = AddComponent<CircleCollider>();

		spriteRenderer->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Attack_Effect.png"), "Attack_Effect");
		spriteRenderer->layer = 1;

	}
	~PlayerAttackArea() override
	{

	}

	void Awake() override
	{
		transform->SetPosition(0.0f, 30.0f);
		transform->SetRotation(0.0f);

		collider->radius = 10.0f;
		collider->isTrigger = true;
	}

	void Update() override
	{

	}
};