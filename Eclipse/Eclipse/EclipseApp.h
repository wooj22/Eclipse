#pragma once
#include "../Direct2D_EngineLib/GameApp.h"

class EclipseApp : public GameApp
{
public:
	enum SceneName { MOON, YUNMO, ARON, WOO, MENU, PLAY, END };	// scene enum

	EclipseApp(int inputWidth, int inputHeight)
		: GameApp(inputWidth, inputHeight) {
	};
	~EclipseApp() override {}

	void Init() override;
	void UnInit() override;
	void MessageProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};