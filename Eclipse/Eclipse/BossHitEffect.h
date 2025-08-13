#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "BCAnimatorController.h"

// Boss�� Hon�� �ε������� ������ effect
// �����ϱ� ������ Script Component ���� �ȸ���

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
		// TODO :: animation ��� �ѹ� ������ destory
	}
};

