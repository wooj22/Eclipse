#include "EclipseApp.h"
#include "../Direct2D_EngineLib/RectTransform.h"
#include "Moon_Scene.h"
#include "Yunmo_Scene.h"
#include "Aron_Scene.h"
#include "Woo_Scene.h"
#include "MenuScene.h"
#include "PlayScene.h"
#include "EndScene.h"

void EclipseApp::Init()
{
	__super::Init();

	// matrix init
	Transform::unityMatrix =
		D2D1::Matrix3x2F::Scale(1.0f, -1.0f) * Transform::unityMatrix *
		D2D1::Matrix3x2F::Translation(width / 2.0f, height / 2.0f);
	RectTransform::unityMatrix =
		D2D1::Matrix3x2F::Scale(1.0f, -1.0f) * D2D1::Matrix3x2F::Translation(width / 2.0f, height / 2.0f);

	// scene init
	sceneManager.CreateScene<Moon_Scene>();
	sceneManager.CreateScene<Yunmo_Scene>();
	sceneManager.CreateScene<Aron_Scene>();
	sceneManager.CreateScene<Woo_Scene>();
	sceneManager.CreateScene<MenuScene>();
	sceneManager.CreateScene<PlayScene>();
	sceneManager.CreateScene<EndScene>();
	sceneManager.SetCurrentScene(MOON);
	sceneManager.Init();
}


void EclipseApp::UnInit()
{
	__super::UnInit();
}


void EclipseApp::MessageProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	__super::MessageProc(hwnd, uMsg, wParam, lParam);

	// game에 활용할 message 처리
	switch (uMsg)
	{
	case WM_CREATE:
		break;
	}
}