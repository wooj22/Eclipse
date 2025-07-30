#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/UI_Text.h"
#include "PlayUI.h"

enum class ChatCondition { None, Success, Fail };

struct WaveChatGroup {
	int waveIndex;
	ChatCondition condition;
	std::vector<std::wstring> lines;
};

class Chat : public Script
{
private:
	std::vector<WaveChatGroup> chatGroups;
	std::vector<std::wstring> currentLines;
	ChatCondition chatCondition;
	int chatCount = 0;				// 대화 카운트 
	bool finished = false;
	UI_Text* chatText;

public:
	void Awake() override;
	void Update() override;
	void AddChat(int waveIndex, ChatCondition condition, const std::wstring& line);
	void AddChatCount() { chatCount++; }
	bool GetFinished() { return finished; }
	void SetCondition(ChatCondition condition);
};

