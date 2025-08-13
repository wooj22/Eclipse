#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/Scene.h"
#include "BossAnimatorController.h"
#include "BossController.h"
#include "BossFace.h"

/* 보스 웨이브에 등장하는 Boss GameObject 선언 클래스 */

class Boss : public GameObject
{
public:
	// components
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	Rigidbody* rigidbody;
	CircleCollider* collider;
	Animator* animator;
	BossController* bossController;

	// animator controller
	BossAnimatorController* bossAniController;

	// child gameobject
	BossFace* bossFace;

	Boss()
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		rigidbody = AddComponent<Rigidbody>();
		collider = AddComponent<CircleCollider>();
		animator = AddComponent<Animator>();
		bossController = AddComponent<BossController>();

		name = "Boss";
		tag = "Boss";

		bossAniController = new BossAnimatorController();
		animator->SetController(bossAniController);
		spriteRenderer->layer = 10;

		transform->SetPosition(0, 500);
		transform->SetScale(0.65, 0.65);

		rigidbody->isKinematic = true;
		collider->isTrigger = true;
		collider->radius = 430;
		collider->offset = { 0,-40 };

		// color matrix test
		spriteRenderer->renderMode = RenderMode::Lit_ColorTint;
		spriteRenderer->SetGlowAmmount(100);

		// boss face
		bossFace =  SceneManager::Get().GetCurrentScene()->CreateObject<BossFace>();
		bossFace->transform->SetParent(this->transform);
	}
};

