#include "Quest.h"


void Quest::Awake()
{
	questLines.push_back(L"");//���̺� ī���Ϳ� ������ �ε�����
	questLines.push_back(L"��ȥ�� 50ȸ ��ȭ(óġ)�ϼ���.");
	questLines.push_back(L"���⸦ 30ȸ �����ϼ���.");
	questLines.push_back(L"�糪�� ��ȣ�ϼ���.");
	questLines.push_back(L"�콺�� óġ�ϼ���.");

	questMaxCount.push_back(L"");//���̺� ī���Ϳ� ������ �ε�����
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