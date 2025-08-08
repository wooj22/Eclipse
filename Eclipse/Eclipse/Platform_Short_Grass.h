#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/BoxCollider.h"

class Platform_Short_Grass : public GameObject
{
public:
    Transform* transform;
    SpriteRenderer* spriteRenderer;
    BoxCollider* collider;

    Platform_Short_Grass() : GameObject("Ground", "Platform_Short_Grass")
    {
        transform = AddComponent<Transform>();
        spriteRenderer = AddComponent<SpriteRenderer>();
        collider = AddComponent<BoxCollider>();
    }
    ~Platform_Short_Grass() override {};

    void Awake() override
    {
        transform->SetScale(0.5, 0.5);

        spriteRenderer->sprite
            = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Platform_Short_Grass.png"), "Platform_Short_Grass");
        spriteRenderer->layer = 1;

        collider->size = { 580.0f, 100.0f };
        collider->isFlatform = true;
    }

    void Update() override
    {
        collider->DebugColliderDraw();
    }
};