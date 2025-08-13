#include "Quest.h"


void Quest::Awake()
{
	questLines.push_back(L"");//웨이브 카운터와 동일한 인덱스용
	questLines.push_back(L"좌측 이동 (A)\n우측 이동 (D)\n점프 (SPACE)");
	questLines.push_back(L"기본공격 (좌 클릭)\n불릿타임 (좌 홀딩)\n대시 (SHIFT)");
	questLines.push_back(L"영혼을 20회\n정화(처치)하세요.");
	questLines.push_back(L"TAP을 눌러 \n스킬을 배우세요");
	questLines.push_back(L"");
	questLines.push_back(L"연쇄를 10회\n진행하세요.");
	questLines.push_back(L"");
	questLines.push_back(L"");
	questLines.push_back(L"루나를 보호하세요.");
	questLines.push_back(L"");
	questLines.push_back(L"녹스를 처치하세요.");

	questMaxCount.push_back(0);//웨이브 카운터와 동일한 인덱스용
	questMaxCount.push_back(20);
	questMaxCount.push_back(10);
	questMaxCount.push_back(5);
	questMaxCount.push_back(1);

	questReward.push_back(0);//웨이브 카운터와 동일한 인덱스용
	questReward.push_back(100);
	questReward.push_back(150);
	questReward.push_back(200);
	questReward.push_back(0);
}

void Quest::Update()
{

}

void Quest::RefreshQuestText()
{
	if(GameManager::Get().questState != ChatCondition::Quest)
		GameManager::Get().g_playUI->quest_Text->screenTextRenderer->SetText(questLines[GameManager::Get().questIndex]);
	else
	{
		GameManager::Get().g_playUI->quest_Text->screenTextRenderer->SetText(questLines[GameManager::Get().questIndex]);
	}
}

void Quest::RefreshQuestCountText(int num)
{
	if (num == -1)
		GameManager::Get().g_playUI->questCount_Text->screenTextRenderer->SetText(L"");
	else
		GameManager::Get().g_playUI->questCount_Text->screenTextRenderer->SetText(L"[   "+ to_wstring(num) + L"    /    " + to_wstring(questMaxCount[GameManager::Get().waveCount]) + L"   ]");
}

void Quest::QuestSuccess()
{
	GameManager::Get().questState = ChatCondition::Success;
	GameManager::Get().g_playUI->questCount_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
}

void Quest::QuestFail()
{
	GameManager::Get().questState = ChatCondition::Fail;
	GameManager::Get().g_playUI->questCount_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
}
