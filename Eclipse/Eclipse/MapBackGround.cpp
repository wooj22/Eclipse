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

	parallax->parallaxFactor = 0.5;
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

	parallax->parallaxFactor = 0.2;
}