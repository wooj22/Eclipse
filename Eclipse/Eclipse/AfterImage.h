#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"


class AfterImage : public Script
{
private:
    float lifeTime = 0.5f;
    float timer = 0.0f;
    float initialAlpha = 1.0f;

    SpriteRenderer* renderer = nullptr;

public:
    void Awake() override
    {
        renderer = gameObject->GetComponent<SpriteRenderer>();
    }

    void Update() override
    {
        if (!renderer) return;

        // OutputDebugStringA("AfterImage Update 되는중.. \n");

        timer += Time::GetDeltaTime();
        float t = timer / lifeTime;
        float currentAlpha = initialAlpha * (1.0f - t); // 점점 사라짐
        renderer->SetAlpha(currentAlpha);

        if (timer >= lifeTime)
        {
            gameObject->Destroy();
        }
    }

    void SetInitialAlpha(float alpha)
    {
        initialAlpha = alpha;
    }
};