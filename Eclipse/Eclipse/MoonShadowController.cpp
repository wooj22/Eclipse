#include "MoonShadowController.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/InvokeSystem.h"
#include "GameManager.h"
#include "MoonLight.h"
#include "GameManager.h"
#include "PlayUI.h"

void MoonShadowController::Awake() 
{
	tr = gameObject->transform;
	moonTr = GameObject::Find("Moon")->transform;

	tr->SetScale(0.95, 0.95);
}

void MoonShadowController::Update()
{
	// 1~3 wave
	isWaving = GameManager::Get().isWave;
	if (!isStop && isWaving)
	{
		tr->Translate(Vector2::left * Time::GetDeltaTime() * speed);
	}

	// boss wave 시작 전 연출
	if (!isStop && isBossWave)
	{
		tr->Translate(Vector2::left * Time::GetDeltaTime() * speed * 15);
		// 월식
		if (tr->GetWorldPosition().x <= moonTr->GetWorldPosition().x)
		{
			isStop = true;
			tr->SetPosition(moonTr->GetWorldPosition());
			Instantiate<MoonLight>(moonTr->GetWorldPosition());
			
			// boss wave start
			GameManager::Get().g_playUI->BossIntroEnd();

			// 연출 마저
			InvokeSystem::Invoke(2, [this]() { AfterMovingStart(); });
		}
	}

	// boss wave
	if (isAfterMoving && isWaving)
	{
		tr->Translate(Vector2::left * Time::GetDeltaTime() * speed);
	}
}

void MoonShadowController::DirectingBossWave()
{
	isBossWave = true;
}


void MoonShadowController::AfterMovingStart()
{
	isAfterMoving = true;
}