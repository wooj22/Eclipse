#include "Quest.h"


void Quest::Awake()
{
	questLines.push_back(L"영혼을 30회 정화하세요.");
	questLines.push_back(L"연쇄 30회 진행하세요.");
	questLines.push_back(L"루나를 보호하세요.");
	questLines.push_back(L"녹스를 처치하세요.");
}

void Quest::Update()
{
	if (GameManager::Get().waveCount != 0 && GameManager::Get().waveCount < 5)
	{
		GameManager::Get().g_playUI->quest_Text->screenTextRenderer->SetText(questLines[GameManager::Get().waveCount - 1]);
	}
}
