#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"


/* 보스 표정 GameObject */
// Boss GameObject 생성자에서 생성함

class BossFace : public GameObject
{
public:
	// components
	Transform* transform;
	SpriteRenderer* spriteRenderer;

	BossFace()
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();

		name = "BossFace";
		spriteRenderer->layer = 11;
	}
};

