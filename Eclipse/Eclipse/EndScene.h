#pragma once
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/Time.h"
#include <vector>

class EndScene : public Scene
{
private:
	GameObject* cam;
	std::vector<UI_Text*> credits;
	float scrollY = -1080.0f;
	float scrollSpeed = 100.0f;

public:
	EndScene() = default;
	~EndScene() = default;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Exit() override;
};

