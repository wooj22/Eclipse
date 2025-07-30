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

// Forward declaration for Honmun class
class Honmun;

class Aron_Scene : public Scene
{
public:
	// Scene을(를) 통해 상속됨
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void Exit() override;

private:
	// camera
	GameObject* cam = nullptr;

	// UI
	UI_Text* title_text = nullptr;

	// 혼문 enemies (Honmun 클래스 사용)
	Honmun* honmun_a = nullptr;
	Honmun* honmun_b = nullptr;
	Honmun* honmun_c = nullptr;
	Honmun* honmun_d = nullptr;

	// ground for enemies to stand on
	GameObject* ground = nullptr;
	BoxCollider* ground_col = nullptr;

	// collision detection function
	void CheckCollisionAndChangeColor();
};