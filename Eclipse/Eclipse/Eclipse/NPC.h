#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/BoxCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"

#include "PlayUI.h"

class NPC : public GameObject
{
public:
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	Rigidbody* rigidbody;
	BoxCollider* collider;


	MultiDelegate<> interationListeners;

public:
	// game object cycle
	NPC();
	~NPC() override {}

	// 오브젝트가 생성될 때
	void Awake() override;

	// Scene의 Start (씬의 모든 오브젝트가 생성되고 난 뒤)
	void SceneStart() override;

	// Scene의 Update
	void Update()  override;

	// Scene의 Exit, GameObject Delete
	void Destroyed() override;
	//void PlayerInteraction();
};