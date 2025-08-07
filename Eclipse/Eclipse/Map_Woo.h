#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/BoxCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"

class Map_Woo : public GameObject
{
public:
	// components
	Transform* transform;
	BoxCollider* collider;

	Map_Woo() : GameObject("Ground", "EndLine")
	{
		transform = AddComponent<Transform>();
		collider = AddComponent<BoxCollider>();
		collider->size = { 2560, 100 };
	}

	void Update() override
	{
		collider->DebugColliderDraw();
	}
};

