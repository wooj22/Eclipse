#pragma once
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/BoxCollider.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "WaveSystem.h"

class Aron_Scene : public Scene
{
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void Exit() override;

private:
	// Camera
	GameObject* cam = nullptr;

	// UI Elements
	UI_Text* title_text = nullptr;
	UI_Text* wave_status_text = nullptr;
	UI_Text* timer_text = nullptr;
	UI_Text* progress_text = nullptr;
	UI_Text* debug_text = nullptr;

	// Ground system
	GameObject* ground = nullptr;
	BoxCollider* ground_col = nullptr;
	GameObject* floor2 = nullptr;
	BoxCollider* floor2_col = nullptr;

	// Wave System
	WaveSystem* waveSystem = nullptr;

	// Score System
	int currentScore = 0;

	// Helper functions
	void HandleCameraMovement();
	void InitializeWaveSystem();
	void UpdateWaveUI();
	void StartWave(int waveIndex);
	void ResetScene();
	void SetupFonts();
	
public:
	// Score functions
	void AddScore(int points);
	int GetScore() const { return currentScore; }
	void UpdateScoreUI();
};