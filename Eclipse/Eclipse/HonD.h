#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "HonDController.h"

/* Luna ´ÞºûÀÇ È¥ */

class HonD : public GameObject
{
public:
	// components
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	Rigidbody* rigidbody;
	CircleCollider* collider;
	HonDController* controller;

	HonD() : GameObject("HonD", "Hon")		// name, tag
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		rigidbody = AddComponent<Rigidbody>();
		collider = AddComponent<CircleCollider>();
		controller = AddComponent<HonDController>();

		// renderer set
		auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Sample/HonD.png");
		auto new_sprite = ResourceManager::Get().CreateSprite(texture, "HonD");
		spriteRenderer->sprite = new_sprite;
		spriteRenderer->renderMode = RenderMode::Lit_Glow;
		spriteRenderer->layer = 11;

		// physics set
		rigidbody->useGravity = false;
		rigidbody->isKinematic = true;
		collider->isTrigger = true;
		collider->offset = { 0, -5 };
		collider->radius = 40;
	}

	void Update() override
	{
		collider->DebugColliderDraw();
	}
};

