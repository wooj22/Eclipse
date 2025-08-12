#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include "ParallaxLayer.h"
#include "MoonShadowController.h"

/* 월식 그림자 */

class MoonShadow : public GameObject
{
public:
	// components
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	ParallaxLayer* parallax;
	MoonShadowController* controller;

	MoonShadow() : GameObject("MoonShadow", "MoonShadow")
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		parallax = AddComponent<ParallaxLayer>();
		controller = AddComponent<MoonShadowController>();

		// sprite
		auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Environment/BG02_E.png");
		auto new_sprite = ResourceManager::Get().CreateSprite(texture, "MoonShadow");
		spriteRenderer->sprite = new_sprite;
		spriteRenderer->layer = -7;		// moon -9, light -8, shadow -7

		// parallax
		parallax->parallaxFactor = 0.8;
	}
};

