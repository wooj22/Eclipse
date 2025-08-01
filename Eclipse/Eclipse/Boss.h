#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "BossController.h"

/* 보스 웨이브에 등장하는 Boss GameObject 선언 클래스 */

class Boss : public GameObject
{
public:
	// components
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	Rigidbody* rigidbody;
	CircleCollider* collider;
	BossController* bossController;
	
	Boss()
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		rigidbody = AddComponent<Rigidbody>();
		collider = AddComponent<CircleCollider>();
		bossController = AddComponent<BossController>();

		name = "Boss";
		tag = "Boss";

		transform->SetPosition(0, 500);
		transform->SetScale(1, 1);

		auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Sample/Boss.png");
		auto new_sprite = ResourceManager::Get().CreateSprite(texture, "Boss");
		spriteRenderer->sprite = new_sprite;
		spriteRenderer->layer = 10;

		rigidbody->isKinematic = true;
		collider->isTrigger = true;
		collider->radius = 180;
		collider->offset = { 0,-40 };

		spriteRenderer->renderMode = RenderMode::Lit_ColorTint;
		spriteRenderer->SetGlowAmmount(150);
	}

	void Update() override
	{
		collider->DebugColliderDraw();
	}
};

