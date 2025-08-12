#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"

#include "PlayerAreaController.h";
#include "AttackAnimatorController.h"

class PlayerAttackArea : public GameObject
{
	// [ components ]
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	CircleCollider* collider;

	PlayerAreaController* controller;

	// [ animation ]
	Animator* animator;  
	AttackAnimatorController* animController;

	// PlayerFSM* playerFSM = nullptr; // 플레이어 FSM

public:
	PlayerAttackArea() : GameObject("PlayerAttackArea", "PlayerAttackArea")
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		collider = AddComponent<CircleCollider>();
		controller = AddComponent<PlayerAreaController>();

		// Animator & Controller 추가
		animator = AddComponent<Animator>();
		animController = new AttackAnimatorController();
		animator->SetController(animController);

		spriteRenderer->layer = 2;
	}
	~PlayerAttackArea() override
	{
		delete animController;
	}

	void Awake() override
	{
		transform->SetPosition(-200.0f, 0.0f);
		transform->SetRotation(0.0f);
		transform->SetScale(1.3f, 1.3f); 

		collider->offset = { 50.0f, 0.0f };
		collider->radius = 130.0f; 
		collider->isTrigger = true;

		spriteRenderer->renderMode = RenderMode::Lit_ColorTint;

		// 초기 비활성화
		spriteRenderer->SetEnabled(false);
		collider->SetEnabled(false);
	}

	void Update() override
	{
		// AABB 영역 
		if(collider->IsEnabled()) collider->DebugColliderDraw();

		// collider->DebugColliderDraw();
	}

	// 어택 범위 활성화 + 애니메이션 실행
	void Activate()
	{
		spriteRenderer->SetEnabled(true);
		collider->SetEnabled(true);
	}

	void Deactivate()
	{
		spriteRenderer->SetEnabled(false);
		collider->SetEnabled(false);
	}

};