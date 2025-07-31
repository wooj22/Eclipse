#include "Quest.h"


void Quest::Awake()
{
	questLines.push_back(L"영혼을 n회 처치하세요.");
	questLines.push_back(L"연쇄를 n회 진행하세요.");
	questLines.push_back(L"새로 등장하는 영혼을 n회 미만으로 처치하세요.");
	questLines.push_back(L"보스를 처치하세요.");
}

void Quest::Update()
{
	if (GameManager::Get().waveCount != 0 && GameManager::Get().waveCount < 5)
	{
		GameManager::Get().g_playUI->quest_Text->screenTextRenderer->SetText(questLines[GameManager::Get().waveCount - 1]);
	}
}
