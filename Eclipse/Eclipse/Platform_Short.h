#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"

class Platform_Short : public GameObject
{
public:
    Transform * transform;
    SpriteRenderer* spriteRenderer;

    Platform_Short() : GameObject("Platform_Short", "Platform_Short")
    {
        transform = AddComponent<Transform>();
        spriteRenderer = AddComponent<SpriteRenderer>();
    }
    ~Platform_Short() override {};

    void Awake() override
    {
        transform->SetScale(0.5, 0.5);
        spriteRenderer->sprite 
            = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/short_platform.png"), "short_platform");
        spriteRenderer->layer = 1;
    }

    void Update() override
    {

    }

    void SetAlpha(float alpha)
    {
        spriteRenderer->SetAlpha(alpha);
    }
};