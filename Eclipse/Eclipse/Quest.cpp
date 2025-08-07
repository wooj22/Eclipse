#include "Quest.h"


void Quest::Awake()
{
	questLines.push_back(L"");//���̺� ī���Ϳ� ������ �ε�����
	questLines.push_back(L"��ȥ�� 50ȸ ��ȭ(óġ)�ϼ���.");
	questLines.push_back(L"���⸦ 30ȸ �����ϼ���.");
	questLines.push_back(L"�糪�� ��ȣ�ϼ���.");
	questLines.push_back(L"�콺�� óġ�ϼ���.");

	questMaxCount.push_back(0);//���̺� ī���Ϳ� ������ �ε�����
	questMaxCount.push_back(50);
	questMaxCount.push_back(30);
	questMaxCount.push_back(5);
	questMaxCount.push_back(1);
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
	GameManager::Get().g_playUI->questCount_Text->screenTextRenderer->SetText(L"[   "+ to_wstring(num) + L"    /    " + to_wstring(questMaxCount[GameManager::Get().waveCount]) + L"   ]");
}

void Quest::QuestSuccessCheck()
{
	if(GameManager::Get().questState == ChatCondition::Success)
		GameManager::Get().g_playUI->questCount_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
	else
		GameManager::Get().g_playUI->questCount_Text->screenTextRenderer->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
}
