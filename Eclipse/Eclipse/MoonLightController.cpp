#include "MoonLightController.h"
#include "../Direct2D_EngineLib/Time.h"
#include "GameManager.h"

void MoonLightController::Awake()
{
	tr = this->gameObject->transform;
	tr->SetScale(0.5, 0.5);
}

void MoonLightController::Update()
{
    float deltaTime = Time::GetDeltaTime();
    Vector2 scale = tr->GetScale();

    if (scale.x < 1.0f)
    {
        scale.x += scaleSpeed * deltaTime;
        scale.y += scaleSpeed * deltaTime;
        if (scale.x > 1.0f)
        {
            scale.x = 1.0f;
            scale.y = 1.0f;
        }
    }

    // TODO ::  1 찍으면 다시 0.5까지 어두워지고 스스로 destroy

    tr->SetScale(scale.x, scale.y);
}