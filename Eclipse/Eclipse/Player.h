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
#include "AfterImage.h"

class Player : public GameObject
{
	Ray ray;
	RaycastHit hit;

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

	// [ player setting ]
	float playerGravityScale = 100; 

public:
	float GetPlayerGravityScale() const { return playerGravityScale; }

public:
	Player() : GameObject("Player", "Player")
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		rigidbody = AddComponent<Rigidbody>();
		collider = AddComponent<BoxCollider>();
		animator = AddComponent<Animator>();

		spriteRenderer->layer = 2;

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

		transform->SetPosition(200, -400);
		transform->SetScale(0.5, 0.5);

		collider->offset = { 10.0f, -55.0f };
		collider->size = { 140.0f, 350.0f };
		// collider->isTrigger = false;

		rigidbody->useGravity = true;
		rigidbody->gravityScale = playerFSM->defaultGravity;
		rigidbody->mass = 1.3f;
	}

	void Update() override
	{
		D2D1_POINT_2F start = { 0, 0 };
		D2D1_POINT_2F end = { 0, -240 };
		RenderSystem::Get().DebugDrawLine(start, end, transform->GetScreenMatrix(), 2.0f);

		// ray 
		ray.direction = { Vector2::down };
		ray.origin = transform->GetWorldPosition() - Vector2(0, 120);
		hit = ColliderSystem::Get().Raycast(ray, 300);

		if (hit.collider)
		{
			// hit.point.y;
			std::string debugStr = "[PlayerFSM] hit.collider = " + hit.collider->gameObject->tag + "\n";
			OutputDebugStringA(debugStr.c_str());
		}

		// AABB ¿µ¿ª 
		collider->DebugColliderDraw();
	}
};

