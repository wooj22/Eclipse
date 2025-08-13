#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "HCAnimatorController.h"

// Hon�� Hon�� �ε������� ������ effect
// �����ϱ� ������ Script Component ���� �ȸ���

class HonCollisionEffect : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	Animator* animator;
	HCAnimatorController* animatorController;

	HonCollisionEffect()
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		animator = AddComponent<Animator>();

		animatorController = new HCAnimatorController();
		animator->SetController(animatorController);
	}

	void Update() override
	{
		// TODO :: animation ��� �ѹ� ������ destory
	}
};
