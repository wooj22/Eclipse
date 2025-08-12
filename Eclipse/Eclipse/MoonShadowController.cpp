#include "MoonShadowController.h"
#include "../Direct2D_EngineLib/Time.h"
#include "GameManager.h"
#include "MoonLight.h"

void MoonShadowController::Awake() 
{
	tr = gameObject->transform;
	moonTr = GameObject::Find("Moon")->transform;
}

void MoonShadowController::Update()
{
	isWaving = GameManager::Get().isWave;
	if (!isStop && isWaving)
	{
		// 월식 moving
		if(isBossWave) tr->Translate(Vector2::left * Time::GetDeltaTime() * speed * 15);
		else tr->Translate(Vector2::left * Time::GetDeltaTime() * speed);

		// 월식 끝
		if (tr->GetWorldPosition().x <= moonTr->GetWorldPosition().x)
		{
			isStop = true;
			tr->SetPosition(moonTr->GetWorldPosition());

			// 빛 생성
			Instantiate<MoonLight>(moonTr->GetWorldPosition());
		}
	}
}

void MoonShadowController::DirectingBossWave()
{
	isBossWave = true;
}