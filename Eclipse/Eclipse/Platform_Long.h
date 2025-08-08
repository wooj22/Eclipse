#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/BoxCollider.h"

class Platform_Long : public GameObject
{
public:
    Transform* transform;
    SpriteRenderer* spriteRenderer;
    BoxCollider* collider;

    Platform_Long() : GameObject("Ground", "Platform_Long")
    {
        transform = AddComponent<Transform>();
        spriteRenderer = AddComponent<SpriteRenderer>();
        collider = AddComponent<BoxCollider>();
    }
    ~Platform_Long() override {};

    void Awake() override
    {
        transform->SetScale(0.5, 0.5);

        spriteRenderer->sprite
            = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Platform_Long.png"), "Platform_Long");
        spriteRenderer->layer = 1;

        collider->size = { 1250.0f, 100.0f };
        collider->isFlatform = true;
    }

    void Update() override
    {
        collider->DebugColliderDraw();
    }

};