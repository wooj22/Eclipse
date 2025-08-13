#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Animator.h"

#include "LandingAnimatorController.h"

class PlayerLandingEffect : public GameObject
{
public:
	Transform* transform = nullptr;
	SpriteRenderer* spriteRenderer = nullptr;

	Animator* landingAnimator = nullptr;
	LandingAnimatorController* animController = nullptr;

	PlayerLandingEffect() : GameObject("PlayerLandingEffect", "PlayerLandingEffect")
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();

		landingAnimator = AddComponent<Animator>();
		animController = new LandingAnimatorController();
		landingAnimator->SetController(animController);

		spriteRenderer->layer = 30; // 21;

	}
	~PlayerLandingEffect() override
	{
		delete animController;
	}

	void Awake() override
	{
		transform->SetPosition(0.0f, 0.0f);
		transform->SetScale(1.0f, 1.0f);
	}
};