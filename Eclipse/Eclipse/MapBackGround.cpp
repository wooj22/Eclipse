#include "MapBackGround.h"
#include "ParallaxLayer.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

MapBackGround::MapBackGround()
{
	// componet
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();

	// create child
	backGround00_Sky = SceneManager::Get().GetCurrentScene()->CreateObject<BackGround00_Sky>();
	backGround01_Moon = SceneManager::Get().GetCurrentScene()->CreateObject<BackGround01_Moon>();
	backGround02_Mount = SceneManager::Get().GetCurrentScene()->CreateObject<BackGround02_Mount>();
	backGround03_Tree = SceneManager::Get().GetCurrentScene()->CreateObject<BackGround03_Tree>();

	backGround04_Grass = SceneManager::Get().GetCurrentScene()->CreateObject<BackGround04_Grass>();
	backGround05_Ground = SceneManager::Get().GetCurrentScene()->CreateObject<BackGround05_Ground>();
	backGround06_House = SceneManager::Get().GetCurrentScene()->CreateObject<BackGround06_House>();
	backGround07_Trash = SceneManager::Get().GetCurrentScene()->CreateObject<BackGround07_Trash>();

	backGround08_Light = SceneManager::Get().GetCurrentScene()->CreateObject<BackGround08_Light>();
	backGround09_Shadow = SceneManager::Get().GetCurrentScene()->CreateObject<BackGround09_Shadow>();

	// parent
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

	name = "Moon";
	tag = "Moon";

	auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Environment/BG02.png");
	auto new_sprite = ResourceManager::Get().CreateSprite(texture, "BackGround01_Moon");
	spriteRenderer->sprite = new_sprite;
	spriteRenderer->layer = -9;
	spriteRenderer->renderMode = RenderMode::Lit_Glow;

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
	spriteRenderer->layer = -5;

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
	spriteRenderer->layer = -4;

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
	spriteRenderer->layer = -3;

	parallax->parallaxFactor = 0.2;
}

BackGround05_Ground::BackGround05_Ground()
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();

	auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Environment/BG05.png");
	auto new_sprite = ResourceManager::Get().CreateSprite(texture, "BackGround05_Ground");
	spriteRenderer->sprite = new_sprite;
	spriteRenderer->layer = -2;
}

BackGround06_House::BackGround06_House()
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();

	auto texture = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Environment/BG06.png");
	auto new_sprite = ResourceManager::Get().CreateSprite(texture, "BackGround06_House");
	spriteRenderer->sprite = new_sprite;
	spriteRenderer->layer = -1;
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