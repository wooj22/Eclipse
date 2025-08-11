#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Animator.h"

#include "SkillAnimatorController.h"

class PlayerSkillEffect : public GameObject
{
public:
	// [ Skill Animation ]
	Transform* transform = nullptr;
	SpriteRenderer* spriteRenderer = nullptr;

	Animator* skillAnimator = nullptr;
	SkillAnimatorController* animController = nullptr;

	PlayerSkillEffect() : GameObject("PlayerSkillEffect", "PlayerSkillEffect")
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();

		skillAnimator = AddComponent<Animator>();
		animController = new SkillAnimatorController();
		skillAnimator->SetController(animController);

		spriteRenderer->layer = 2; 

	}
	~PlayerSkillEffect() override
	{
		delete animController;
	}

	void Awake() override
	{
		transform->SetPosition(0.0f, 0.0f);
		transform->SetScale(5.0f, 5.0f); 

		spriteRenderer->renderMode = RenderMode::Lit_ColorTint;
		// spriteRenderer->SetEnabled(false); 
	}
};