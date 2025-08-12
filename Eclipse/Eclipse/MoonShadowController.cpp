#include "MoonShadowController.h"
#include "../Direct2D_EngineLib/Time.h"
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
	isWaving = GameManager::Get().isWave;
	if (!isStop && isWaving)
	{
		// ���� moving
		tr->Translate(Vector2::left * Time::GetDeltaTime() * speed);
	}


	if (!isStop && isBossWave)
	{
		tr->Translate(Vector2::left * Time::GetDeltaTime() * speed * 15);
		// ���� ��
		if (tr->GetWorldPosition().x <= moonTr->GetWorldPosition().x)
		{
			isStop = true;
			tr->SetPosition(moonTr->GetWorldPosition());

			// �� ����
			Instantiate<MoonLight>(moonTr->GetWorldPosition());

			// boss wave start
			GameManager::Get().g_playUI->BossIntroEnd();
		}
	}
}

void MoonShadowController::DirectingBossWave()
{
	isBossWave = true;
}