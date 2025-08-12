#include "MoonShadowController.h"
#include "../Direct2D_EngineLib/Time.h"
#include "GameManager.h"

void MoonShadowController::Awake() 
{
	tr = gameObject->transform;
}

void MoonShadowController::Update()
{
	isWaving = GameManager::Get().isWave;
	if (!isStop && isWaving)
	{
		if(isBossWave) tr->Translate(Vector2::left * Time::GetDeltaTime() * speed * 15);
		else tr->Translate(Vector2::left * Time::GetDeltaTime() * speed);

		if (tr->GetWorldPosition().x <= 0)
		{
			isStop = true;
			tr->SetPosition(0, 0);
		}
	}
}

void MoonShadowController::DirectingBossWave()
{
	isBossWave = true;
}