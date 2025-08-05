#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "HonBAnimatorController.h"
#include "HonBController.h"

/* Umbra �׸����� ȥ */

class HonB: public GameObject
{
public:
	// components
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	Rigidbody* rigidbody;
	CircleCollider* collider;
	Animator* animator;
	HonBController* controller;

	// animator controller asset
	HonBAnimatorController* animatorController;

	HonB() : GameObject("HonB", "Hon")		// name, tag
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		rigidbody = AddComponent<Rigidbody>();
		collider = AddComponent<CircleCollider>();
		animator = AddComponent<Animator>();
		controller = AddComponent<HonBController>();
		
		// animator
		animatorController = new HonBAnimatorController();
		animator->SetController(animatorController);

		// renderer set
		spriteRenderer->renderMode = RenderMode::Lit_Glow;
		spriteRenderer->layer = 11;

		// physics set
		rigidbody->useGravity = false;
		rigidbody->isKinematic = true;
		collider->isTrigger = true;
		collider->offset = { 0, -15 };
		collider->radius = 40;
	}

	~HonB()
	{
		delete animatorController;
	}

	void Update() override
	{
		collider->DebugColliderDraw();
	}
};

