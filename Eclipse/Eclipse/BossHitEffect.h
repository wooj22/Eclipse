#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "BCAnimatorController.h"

// Boss랑 Hon이 부딪혔을때 나오는 effect
// 간단하기 때문에 Script Component 따로 안만듦

class BossHitEffect : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	Animator* animator;
	BCAnimatorController* animatorController;

	BossHitEffect()
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		animator = AddComponent<Animator>();

		animatorController = new BCAnimatorController();
		animator->SetController(animatorController);
	}

	void Update() override
	{
		// TODO :: animation 재생 한번 끝나면 destory
	}
};

