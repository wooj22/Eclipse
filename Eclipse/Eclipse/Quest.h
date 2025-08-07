#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/UI_Text.h"
#include "GameManager.h"
#include "PlayUI.h"

class Quest : public Script
{
private:
	std::vector<std::wstring> questLines;
	std::vector<int> questMaxCount;
	std::vector<int> questReward;
public:
	void Awake() override;
	void Update() override;

	void RefreshQuestText();
	void RefreshQuestCountText(int num);
	int GetQuestMaxCount() { return questMaxCount[GameManager::Get().waveCount]; }
	void QuestSuccess();
	void QuestFail();
	int QuestReward() { return questReward[GameManager::Get().waveCount];}
};

