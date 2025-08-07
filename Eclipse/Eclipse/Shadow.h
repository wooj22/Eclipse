#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"

class Shadow : public GameObject
{
public:
    Transform* transform;
    SpriteRenderer* spriteRenderer;

    Shadow() : GameObject("Shadow", "Shadow")
    {
        transform = AddComponent<Transform>();
        spriteRenderer = AddComponent<SpriteRenderer>();
    }
    ~Shadow() override {};

    void Awake() override
    {
        transform-> SetScale(0.6, 0.5);
        spriteRenderer->sprite = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Shadow.png"), "Shadow");
        spriteRenderer->renderMode = RenderMode::Lit_ColorTint;
        spriteRenderer->layer = 2; 
        spriteRenderer->SetAlpha(0.5f);
    }

    void Update() override
    {

    }

    void SetAlpha(float alpha)
    {
        spriteRenderer->SetAlpha(alpha);
    }
};
