#include "MapBackGround.h"
#include "ParallaxLayer.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

MapBackGround::MapBackGround()
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
}

void MapBackGround::SceneStart()
{
	backGround00_Sky->transform->SetParent(transform);
	backGround01_Moon->transform->SetParent(transform);
	backGround02_Mount->transform->SetParent(transform);
	backGround03_Tree->transform->SetParent(transform);
	backGround04_Grass->transform->SetParent(transform);
	backGround05_Ground->transform->SetParent(transform);
	backGround06_House->transform->SetParent(transform);
	backGround07_Trash->transform->SetParent(transform);
	backGround08_Light->transform->SetParent(transform);
	backGround09_Shadow->transform->SetParent(transform);
}

BackGround00_Sky::BackGround00_Sky()
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
	parallax = AddComponent<ParallaxLayer>();

	auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Environment/BG00.png");
	auto new_sprite = ResourceManager::Get().CreateSprite(texture, "BackGround00_Sky");
	spriteRenderer->sprite = new_sprite;
	spriteRenderer->layer = -10;

	parallax->parallaxFactor = 0.9;
}

BackGround01_Moon::BackGround01_Moon()
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
	parallax = AddComponent<ParallaxLayer>();

	auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Environment/BG02.png");
	auto new_sprite = ResourceManager::Get().CreateSprite(texture, "BackGround01_Moon");
	spriteRenderer->sprite = new_sprite;
	spriteRenderer->layer = -9;

	parallax->parallaxFactor = 0.8;
}

BackGround02_Mount::BackGround02_Mount()
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
	parallax = AddComponent<ParallaxLayer>();

	auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Environment/BG01.png");
	auto new_sprite = ResourceManager::Get().CreateSprite(texture, "BackGround02_Mount");
	spriteRenderer->sprite = new_sprite;
	spriteRenderer->layer = -8;

	parallax->parallaxFactor = 0.7;
}

BackGround03_Tree::BackGround03_Tree()
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
	parallax = AddComponent<ParallaxLayer>();

	auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Environment/BG03.png");
	auto new_sprite = ResourceManager::Get().CreateSprite(texture, "BackGround03_Tree");
	spriteRenderer->sprite = new_sprite;
	spriteRenderer->layer = -7;

	parallax->parallaxFactor = 0.3;
}

BackGround04_Grass::BackGround04_Grass()
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
	parallax = AddComponent<ParallaxLayer>();

	auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Environment/BG04.png");
	auto new_sprite = ResourceManager::Get().CreateSprite(texture, "BackGround04_Grass");
	spriteRenderer->sprite = new_sprite;
	spriteRenderer->layer = -6;

	parallax->parallaxFactor = 0.2;
}

BackGround05_Ground::BackGround05_Ground()
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();

	auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Environment/BG05.png");
	auto new_sprite = ResourceManager::Get().CreateSprite(texture, "BackGround05_Ground");
	spriteRenderer->sprite = new_sprite;
	spriteRenderer->layer = -5;
}

BackGround06_House::BackGround06_House()
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();

	auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Environment/BG06.png");
	auto new_sprite = ResourceManager::Get().CreateSprite(texture, "BackGround06_House");
	spriteRenderer->sprite = new_sprite;
	spriteRenderer->layer = -4;
}

BackGround07_Trash::BackGround07_Trash()
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();

	auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Environment/BG07_FG.png");
	auto new_sprite = ResourceManager::Get().CreateSprite(texture, "BackGround07_Trash");
	spriteRenderer->sprite = new_sprite;
	spriteRenderer->layer = 30;
}

BackGround08_Light::BackGround08_Light()
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
	parallax = AddComponent<ParallaxLayer>();

	auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Environment/BG_light.png");
	auto new_sprite = ResourceManager::Get().CreateSprite(texture, "BackGround08_Light");
	spriteRenderer->sprite = new_sprite;
	spriteRenderer->layer = 30;

	parallax->parallaxFactor = 0.45;
}

BackGround09_Shadow::BackGround09_Shadow()
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();

	auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Environment/BG08_FG2.png");
	auto new_sprite = ResourceManager::Get().CreateSprite(texture, "BackGround09_Shadow");
	spriteRenderer->sprite = new_sprite;
	spriteRenderer->layer = 30;
}