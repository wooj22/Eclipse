#include "Quest.h"


void Quest::Awake()
{
	questLines.push_back(L"��ȥ�� nȸ óġ�ϼ���.");
	questLines.push_back(L"���⸦ nȸ �����ϼ���.");
	questLines.push_back(L"���� �����ϴ� ��ȥ�� nȸ �̸����� óġ�ϼ���.");
	questLines.push_back(L"������ óġ�ϼ���.");
}

void Quest::Update()
{
	if (GameManager::Get().waveCount != 0 && GameManager::Get().waveCount < 5)
	{
		GameManager::Get().g_playUI->quest_Text->screenTextRenderer->SetText(questLines[GameManager::Get().waveCount - 1]);
	}
}
