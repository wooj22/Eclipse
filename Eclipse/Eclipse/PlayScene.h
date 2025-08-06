#pragma once
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/UI_Text.h"

#include "PlayUI.h"
#include "EclipseApp.h"
#include "NPC.h"
#include "Player_Test.h"
#include "SkillWindowButton.h"
#include "WaveSystem.h"

class PlayScene : public Scene
{
private:
	GameObject* cam;
	UI_Text* title_text;
	GameObject* backGround;
	Player_Test* player;
	NPC* npc;

	GameObject* waveSystemObj = nullptr;
	WaveSystem* waveSystem = nullptr;

	PlayUI* playUI;

public:
	PlayScene() = default;
	~PlayScene() = default;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Exit() override;

private:
	void ChagneEndScene();
};

