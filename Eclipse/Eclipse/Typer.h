#pragma once
#include "../Direct2D_EngineLib/Script.h"

using namespace std;

// 타이핑 연출 스크립트

class ScreenTextRenderer;
class Typer : public Script
{
private:
	ScreenTextRenderer* textRenderer = nullptr;
	wstring target_text = L"";
	wstring cur_text = L"";
	float typingSpeed = 10.0f;
	float spacingDelay = 0.3f;
	float timer = 0.0f;
	bool isTyping;

	void TypingDirecting();

public:
	// cycle
	void Update();

	// funcs
	void StartTyping(wstring text);
	void EndTyping();

	// setter
	void SetTextRenderer(ScreenTextRenderer* st) { if(st) textRenderer = st; }

	// getter
	bool IsTypingEnd() { return !isTyping; }
};

