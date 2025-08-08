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

class Platform_Long_Grass : public GameObject
{
public:
    Transform* transform;
    SpriteRenderer* spriteRenderer;
    BoxCollider* collider;

    Platform_Long_Grass() : GameObject("Ground", "Platform_Long_Grass")
    {
        transform = AddComponent<Transform>();
        spriteRenderer = AddComponent<SpriteRenderer>();
        collider = AddComponent<BoxCollider>();
    }
    ~Platform_Long_Grass() override {};

    void Awake() override
    {
        transform->SetScale(0.5, 0.5);

        spriteRenderer->sprite
            = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Platform_Long_Grass.png"), "Platform_Long_Grass");
        spriteRenderer->layer = 1;

        collider->size = { 1250.0f, 100.0f };
        collider->isFlatform = true;
    }

    void Update() override
    {
        collider->DebugColliderDraw();
    }

};

class Platform_Middle : public GameObject
{
public:
    Transform* transform;
    SpriteRenderer* spriteRenderer;
    BoxCollider* collider;

    Platform_Middle() : GameObject("Ground", "Platform_Middle")
    {
        transform = AddComponent<Transform>();
        spriteRenderer = AddComponent<SpriteRenderer>();
        collider = AddComponent<BoxCollider>();
    }
    ~Platform_Middle() override {};

    void Awake() override
    {
        transform->SetScale(0.5, 0.5);

        spriteRenderer->sprite
            = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Platform_Middle.png"), "Platform_Middle");
        spriteRenderer->layer = 1;

        collider->size = { 900.0f, 100.0f };
        collider->isFlatform = true;
    }

    void Update() override
    {
        collider->DebugColliderDraw();
    }
};

class Platform_Middle_Grass : public GameObject
{
public:
    Transform* transform;
    SpriteRenderer* spriteRenderer;
    BoxCollider* collider;

    Platform_Middle_Grass() : GameObject("Ground", "Platform_Middle_Grass")
    {
        transform = AddComponent<Transform>();
        spriteRenderer = AddComponent<SpriteRenderer>();
        collider = AddComponent<BoxCollider>();
    }
    ~Platform_Middle_Grass() override {};

    void Awake() override
    {
        transform->SetScale(0.5, 0.5);

        spriteRenderer->sprite
            = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Platform_Middle_Grass.png"), "Platform_Middle_Grass");
        spriteRenderer->layer = 1;

        collider->size = { 900.0f, 100.0f };
        collider->isFlatform = true;
    }

    void Update() override
    {
        collider->DebugColliderDraw();
    }
};

class Platform_Short : public GameObject
{
public:
    Transform* transform;
    SpriteRenderer* spriteRenderer;
    BoxCollider* collider;

    Platform_Short() : GameObject("Ground", "Platform_Short")
    {
        transform = AddComponent<Transform>();
        spriteRenderer = AddComponent<SpriteRenderer>();
        collider = AddComponent<BoxCollider>();
    }
    ~Platform_Short() override {};

    void Awake() override
    {
        transform->SetScale(0.5, 0.5);

        spriteRenderer->sprite
            = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Platform_Short.png"), "Platform_Short");
        spriteRenderer->layer = 1;

        collider->size = { 580.0f, 100.0f };
        collider->isFlatform = true;
    }

    void Update() override
    {
        collider->DebugColliderDraw();
    }
};

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

class Platform_Wall : public GameObject
{
public:
    Transform* transform;
    SpriteRenderer* spriteRenderer;
    BoxCollider* collider;

    Platform_Wall() : GameObject("Wall", "Platform_Wall")
    {
        transform = AddComponent<Transform>();
        spriteRenderer = AddComponent<SpriteRenderer>();
        collider = AddComponent<BoxCollider>();
    }
    ~Platform_Wall() override {};

    void Awake() override
    {
        transform->SetScale(0.5, 0.5);

        spriteRenderer->sprite
            = ResourceManager::Get().CreateSprite(ResourceManager::Get().CreateTexture2D("../Resource/Moon/Platform_Wall.png"), "Platform_Wall");
        spriteRenderer->layer = 1;

        collider->size = { 100.0f, 790.0f };
        // collider->isFlatform = true;
    }

    void Update() override
    {
        collider->DebugColliderDraw();
    }

};

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