#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/BoxCollider.h"

class BoxObject : public GameObject
{
public:
    Transform* transform;
    SpriteRenderer* spriteRenderer;
    BoxCollider* collider;

    BoxObject() : GameObject("Ground", "BoxObject")
    {
        transform = AddComponent<Transform>();
        spriteRenderer = AddComponent<SpriteRenderer>();
        collider = AddComponent<BoxCollider>();
    }
    ~BoxObject() override {};

    void Awake() override
    {
        transform->SetScale(0.5, 0.4);

        spriteRenderer->sprite
            = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Box.png"), "Box");
        spriteRenderer->layer = 1;

        collider->size = { 300.0f, 300.0f };
        collider->isFlatform = true;
    }

    void Update() override
    {
        collider->DebugColliderDraw();
    }

};