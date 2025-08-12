#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include "ParallaxLayer.h"
#include "MoonLightController.h"

/* 월식이 완성될때 빛 */

class MoonLight : public GameObject
{
public:
	// components
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	ParallaxLayer* parallax;
	MoonLightController* controller;

	MoonLight() : GameObject("MoonLight", "MoonLight")
	{
		transform = AddComponent<Transform>();
		spriteRenderer = AddComponent<SpriteRenderer>();
		parallax = AddComponent<ParallaxLayer>();
		controller = AddComponent<MoonLightController>();

		// sprite
		auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Environment/BG02_L.png");
		auto new_sprite = ResourceManager::Get().CreateSprite(texture, "MoonLight");
		spriteRenderer->sprite = new_sprite;
		spriteRenderer->layer = -8;		// moon -9, light -8, shadow -7

		// parallax
		parallax->parallaxFactor = 0.8;
	}
};

