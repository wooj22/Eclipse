#pragma once
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/UI_Text.h"

class Yunmo_Scene : public Scene
{
private:
	GameObject* cam;
	UI_Text* title_text;

public:
	Yunmo_Scene() = default;
	~Yunmo_Scene() = default;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Exit() override;
};

