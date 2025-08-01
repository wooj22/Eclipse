#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "BulletController.h"

class Bullet : public GameObject
{
public:
	// components
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	Rigidbody* rigidbody;
	CircleCollider* collider;
	BulletController* controller;

	Bullet()
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		rigidbody = AddComponent<Rigidbody>();
		collider = AddComponent<CircleCollider>();
		controller = AddComponent<BulletController>();

		name = "Bullet";
		tag = "Bullet";

		transform->SetScale(0.3, 0.3);

		auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Sample/Boss_Bullet.png");
		auto new_sprite = ResourceManager::Get().CreateSprite(texture, "Bullet");
		spriteRenderer->sprite = new_sprite;
		spriteRenderer->layer = 10;
		spriteRenderer->renderMode = RenderMode::UnlitColorTint;

		rigidbody->isKinematic = true;
		collider->isTrigger = true;
		collider->radius = 60;
	}

	void Update() override
	{
		collider->DebugColliderDraw();
	}
};

