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

	// ������Ʈ�� ������ ��
	void Awake() override;

	// Scene�� Start (���� ��� ������Ʈ�� �����ǰ� �� ��)
	void SceneStart() override;

	// Scene�� Update
	void Update()  override;

	// Scene�� Exit, GameObject Delete
	void Destroyed() override;
	//void PlayerInteraction();
};