#include "Quest.h"


void Quest::Awake()
{
	questLines.push_back(L"��ȥ�� 50ȸ ��ȭ(óġ)�ϼ���.");
	questLines.push_back(L"���⸦ 30ȸ �����ϼ���.");
	questLines.push_back(L"�糪�� ��ȣ�ϼ���.");
	questLines.push_back(L"�콺�� óġ�ϼ���.");
}

void Quest::Update()
{
	if (GameManager::Get().waveCount != 0 && GameManager::Get().waveCount < 5)
	{
		GameManager::Get().g_playUI->quest_Text->screenTextRenderer->SetText(questLines[GameManager::Get().waveCount - 1]);
	}
}
