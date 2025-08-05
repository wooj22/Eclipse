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
#include <vector>
#include <random>

// Forward declarations
class WaveSystem;

class Aron_Scene : public Scene
{
public:
	// Scene interface implementation
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void Exit() override;

private:
	// camera
	GameObject* cam = nullptr;

	// UI
	UI_Text* title_text = nullptr;
	UI_Text* score_text = nullptr;
	UI_Text* debug_text = nullptr;

	// Wave system
	GameObject* waveSystemObj = nullptr;
	WaveSystem* waveSystem = nullptr;

	// ground for enemies to stand on
	GameObject* ground = nullptr;      // Floor 1 (destruction)
	BoxCollider* ground_col = nullptr;
	GameObject* floor2 = nullptr;      // Floor 2 (passthrough)
	BoxCollider* floor2_col = nullptr;

	// Score system
	int currentScore = 0;

	// Camera movement
	void HandleCameraMovement();

public:
	// Score functions
	void AddScore(int points);
	int GetScore() const { return currentScore; }
	void UpdateScoreUI();
	
	// Scene reset
	void ResetScene();
};