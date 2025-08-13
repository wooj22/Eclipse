#pragma once
#include "../Direct2D_EngineLib/Script.h"

using namespace std;

// 타이핑 연출 스크립트
// 사운드 재생도 같이 해줌!

class ScreenTextRenderer;
class AudioSource;
class AudioClip;
class Typer : public Script
{
private:
	// ref component
	ScreenTextRenderer* textRenderer = nullptr;
	AudioSource* audioSource = nullptr;

	// data
	wstring target_text = L"";
	wstring cur_text = L"";
	float typingSpeed = 50.0f;
	float spacingDelay = 0.05f;
	float timer = 0.0f;
	bool isTyping;

	// sound
	const int SOUND_COUNT = 3;
	int soundIndex = 0;
	shared_ptr<AudioClip> typingSound[3];

	// typing
	void TypingDirecting();

public:
	// cycle
	void Awake();
	void Update();

	// funcs
	void StartTyping(wstring text);
	void EndTyping();

	// setter
	void SetTextRenderer(ScreenTextRenderer* st) { if(st) textRenderer = st; }
	void SetAudioSource(AudioSource* as) { if (as) audioSource = as; }

	// getter
	bool IsTypingEnd() { return !isTyping; }
};

