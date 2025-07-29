#pragma once
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/UI_Text.h"

class Aron_Scene : public Scene
{
private:
	GameObject* cam;
	UI_Text* title_text;

public:
	Aron_Scene() = default;
	~Aron_Scene() = default;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Exit() override;
};



