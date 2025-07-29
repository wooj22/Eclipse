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
	GameObject* honmun_a;
	GameObject* honmun_b;
	GameObject* honmun_c;
	GameObject* honmun_d;

	// sprite renderers for color change
	SpriteRenderer* honmun_a_sr;
	SpriteRenderer* honmun_b_sr;
	SpriteRenderer* honmun_c_sr;
	SpriteRenderer* honmun_d_sr;

	// colliders for enemies
	BoxCollider* honmun_a_col;
	BoxCollider* honmun_b_col;
	BoxCollider* honmun_c_col;
	BoxCollider* honmun_d_col;

	// ground for enemies to stand on
	GameObject* ground;
	BoxCollider* ground_col;

	// collision detection function
	void CheckCollisionAndChangeColor();
};