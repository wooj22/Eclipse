#pragma once
#include "../Direct2D_EngineLib/GameApp.h"
#include "GameManager.h"


class EclipseApp : public GameApp
{
private:
	GameManager gameManager;

public:
	enum SceneName { MOON, YUNMO, ARON, WOO, MENU, PLAY, END };	// scene enum
	HCURSOR hCursor;

	EclipseApp(int inputWidth, int inputHeight)
		: GameApp(inputWidth, inputHeight) {
	};
	~EclipseApp() override {}

	void Init() override;
	void UnInit() override;
	void MessageProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};