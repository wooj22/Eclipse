#pragma once
#include "Windows.h"
#include "string"
#include "Singleton.h"
#include "GameObject.h"
#include "RenderSystem.h"
#include "TransformSystem.h"
#include "ColliderSystem.h"
#include "PhysicsSystem.h"
#include "AnimatorSystem.h"
#include "UISystem.h"
#include "ScriptSystem.h"
#include "AudioSystem.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "CameraSystem.h"
#include "Time.h"
#include "Input.h"
#include "InvokeSystem.h"
#include "ObjectTable.h"

using namespace std;

class GameApp
{
protected:
	// window
	HINSTANCE		hInstance;		// 실행중인 프로그램 인스턴스 핸들
	HWND			hWnd;			// 윈도우 핸들
	int				width;
	int				height;
	wstring			winClassName = L"woo";
	wstring			titleName = L"Direct 2D Project";
	string			modulePath;
	string			workingPath;

private:
	// physics update
	float accumulator = 0.0f;
	const float fixedDeltaTime = 0.02f;

protected:
	// object table
	ObjectTable objectTable;

	// component system
	TransformSystem transformSystem;	// transform, rectTransfrom
	RenderSystem renderSystem;			// sprite, image, worldText, screenText
	AnimatorSystem animatorSystem;		// animator
	UISystem uiSystem;					// ui
	ScriptSystem scriptSystem;			// script
	ColliderSystem colliderSystem;		// collider
	PhysicsSystem physicsSystem;		// rigidbody
	CameraSystem cameraSystem;			// camera
	AudioSystem audioSystem;			// sound

	// manager
	SceneManager sceneManager;
	ResourceManager resourceManager;

	static bool isLoop;

public:
	GameApp(int inputWidth = 1920, int inputHeight = 1080) :
		width(inputWidth), height(inputHeight), hInstance(nullptr), hWnd(nullptr) { };
	virtual ~GameApp() = default;

	/* Game Life Cycle */
	virtual void Init();
	void Update();			// input, time, invoke, uiystem, transformSystem, scene(gameObject), scriptsystem.Update(), animatorSystem, renderSystem.update, cameraSystem, script late udapte(), audioSystem
	void FixedUpdate();		// scriptsystem.FixedUpdate(), 물리 관련
	void Render();			// renderSystem.render
	void Loop();
	virtual void UnInit();

	virtual void MessageProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static void Quit()
	{
		isLoop = false;
	}
};

