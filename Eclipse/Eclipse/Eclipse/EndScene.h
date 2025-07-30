#pragma once
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/UI_Text.h"

class EndScene : public Scene
{
private:
	GameObject* cam;
	UI_Text* title_text;

public:
	EndScene() = default;
	~EndScene() = default;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Exit() override;
};

