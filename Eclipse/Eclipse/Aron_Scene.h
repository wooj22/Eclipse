#pragma once
#include "../Direct2D_EngineLib/Scene.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/UI_Text.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/BoxCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"

class Aron_Scene : public Scene
{
public:
	// Scene¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void Exit() override;

private:
	// camera
	GameObject* cam;

	// UI
	UI_Text* title_text;

	// »•πÆ enemies
	GameObject* honmun_a = nullptr;
	GameObject* honmun_b = nullptr;
	GameObject* honmun_c = nullptr;
	GameObject* honmun_d = nullptr;

	// sprite renderers for color change
	SpriteRenderer* honmun_a_sr = nullptr;
	SpriteRenderer* honmun_b_sr = nullptr;
	SpriteRenderer* honmun_c_sr = nullptr;
	SpriteRenderer* honmun_d_sr = nullptr;

	// colliders for enemies
	BoxCollider* honmun_a_col = nullptr;
	BoxCollider* honmun_b_col = nullptr;
	BoxCollider* honmun_c_col = nullptr;
	BoxCollider* honmun_d_col = nullptr;

	// ground for enemies to stand on
	GameObject* ground;
	BoxCollider* ground_col;

	// collision detection function
	void CheckCollisionAndChangeColor();
};