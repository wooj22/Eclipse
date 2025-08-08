#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/BoxCollider.h"

class Platform_Wall_Grass : public GameObject
{
public:
    Transform* transform;
    SpriteRenderer* spriteRenderer;
    BoxCollider* collider;

    Platform_Wall_Grass() : GameObject("Wall", "Platform_Wall_Grass")
    {
        transform = AddComponent<Transform>();
        spriteRenderer = AddComponent<SpriteRenderer>();
        collider = AddComponent<BoxCollider>();
    }
    ~Platform_Wall_Grass() override {};

    void Awake() override
    {
        transform->SetScale(0.5, 0.5);

        spriteRenderer->sprite
            = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Platform_Wall_Grass.png"), "Platform_Wall_Grass");
        spriteRenderer->layer = 1;

        collider->size = { 100.0f, 790.0f };
        // collider->isFlatform = true;
    }

    void Update() override
    {
        collider->DebugColliderDraw();
    }

};