#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Animator.h"

#include "JumpAnimatorController.h"

class PlayerJumpEffect : public GameObject
{
public:
	Transform* transform = nullptr;
	SpriteRenderer* spriteRenderer = nullptr;

	Animator* jumpAnimator = nullptr;
	JumpAnimatorController* animController = nullptr;

	PlayerJumpEffect() : GameObject("PlayerJumpEffect", "PlayerJumpEffect")
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();

		jumpAnimator = AddComponent<Animator>();
		animController = new JumpAnimatorController();
		jumpAnimator->SetController(animController);

		spriteRenderer->layer = 24; // 21;

	}
	~PlayerJumpEffect() override
	{
		delete animController;
	}

	void Awake() override
	{
		transform->SetPosition(0.0f, 0.0f);
		transform->SetScale(0.5f, 0.5f);
	}
};