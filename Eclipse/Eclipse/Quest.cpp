#include "Quest.h"


void Quest::Awake()
{
	questLines.push_back(L"");//웨이브 카운터와 동일한 인덱스용
	questLines.push_back(L"영혼을 50회 정화(처치)하세요.");
	questLines.push_back(L"연쇄를 30회 진행하세요.");
	questLines.push_back(L"루나를 보호하세요.");
	questLines.push_back(L"녹스를 처치하세요.");

	questMaxCount.push_back(L"");//웨이브 카운터와 동일한 인덱스용
	questMaxCount.push_back(L"50");
	questMaxCount.push_back(L"30");
	questMaxCount.push_back(L"1");
	questMaxCount.push_back(L"1");
}

void Quest::Update()
{

}

void Quest::RefreshQuestText()
{
	GameManager::Get().g_playUI->quest_Text->screenTextRenderer->SetText(questLines[GameManager::Get().waveCount]);
}

void Quest::RefreshQuestCountText(int num)
{
	GameManager::Get().g_playUI->questCount_Text->screenTextRenderer->SetText(L"[   "+ to_wstring(num) + L"    /    " + questMaxCount[GameManager::Get().waveCount] + L"   ]");
}