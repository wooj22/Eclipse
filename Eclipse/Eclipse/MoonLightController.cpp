#include "MoonLightController.h"
#include "../Direct2D_EngineLib/Time.h"
#include "GameManager.h"

void MoonLightController::Awake()
{
	tr = this->gameObject->transform;
	tr->SetScale(0.2, 0.2);
    increasing = true;
}

void MoonLightController::Update()
{
    float deltaTime = Time::GetDeltaTime();
    Vector2 scale = tr->GetWorldScale();

    if (increasing)
    {
        scale.x += scaleSpeed * deltaTime;
        scale.y += scaleSpeed * deltaTime;

        if (scale.x >= 1.0f)
        {
            scale.x = 1.0f;
            scale.y = 1.0f;
            increasing = false;
        }
    }
    else
    {
        scale.x -= scaleSpeed * deltaTime;
        scale.y -= scaleSpeed * deltaTime;

        if (scale.x <= 0.5f)
        {
            scale.x = 0.5f;
            scale.y = 0.5f;
            this->gameObject->Destroy();
            return;
        }
    }

    tr->SetScale(scale.x, scale.y);
}