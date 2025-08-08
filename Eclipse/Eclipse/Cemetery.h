#pragma once
// Cemetery
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/BoxCollider.h"

class Cemetery : public GameObject
{
public:
    Transform* transform;
    SpriteRenderer* spriteRenderer;
    BoxCollider* collider;

    Cemetery() : GameObject("Ground", "Cemetery")
    {
        transform = AddComponent<Transform>();
        spriteRenderer = AddComponent<SpriteRenderer>();
        collider = AddComponent<BoxCollider>();
    }
    ~Cemetery() override {};

    void Awake() override
    {
        transform->SetScale(0.5, 0.5);

        spriteRenderer->sprite
            = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Cemetery.png"), "Cemetery");
        spriteRenderer->layer = 1;

        collider->size = { 150.0f, 300.0f };
        collider->isFlatform = true;
    }

    void Update() override
    {
        collider->DebugColliderDraw();
    }
};