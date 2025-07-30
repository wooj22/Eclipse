#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/BoxCollider.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"

#include "PlayerFSM.h"
#include "PlayerAnimatorController.h"

class Player : public GameObject
{
public:
	// [ components ]
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	Rigidbody* rigidbody;
	BoxCollider* collider;
	Animator* animator;

	// [ animation asset ]
	PlayerAnimatorController* playerAnimatorController;

	// [ script ]
	PlayerFSM* playerFSM;

public:
	Player() : GameObject("Player", "Player")
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		rigidbody = AddComponent<Rigidbody>();
		collider = AddComponent<BoxCollider>();
		animator = AddComponent<Animator>();

		auto player = ResourceManager::Get().CreateTexture2D("../Resource/Moon/Player.png");
		spriteRenderer->sprite = ResourceManager::Get().CreateSprite(player, "Player");
		spriteRenderer->layer = 1;

		playerFSM = AddComponent<PlayerFSM>();
	}
	~Player() override
	{
		delete playerAnimatorController;
	};

	void Awake() override // Setting 
	{
		// animator controller
		playerAnimatorController = new PlayerAnimatorController();
		animator->SetController(playerAnimatorController);

		transform->SetPosition(0, 0);
		transform->SetScale(1, 1);

		collider->offset = { 0.0f, -8.0f };
		collider->size = { 60.0f, 90.0f };
		// collider->isTrigger = false;

		rigidbody->useGravity = true;
		rigidbody->gravityScale = 80;
	}

	void Update() override
	{
		// AABB ¿µ¿ª 
		collider->DebugColliderDraw();
	}

};

