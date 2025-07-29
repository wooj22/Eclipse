#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/ICollider.h"

/* ���� ��� �߰� �׽�Ʈ�� */

class Transform;
class SpriteRenderer;
class AnimatorController;
class WorldTextRenderer;
class Rigidbody;

class PlayerController_Woo : public Script
{
private:
	// stat
	float speed = 300;
	float jumpForce = 350.0f;
	float inputX, inputY;
	bool isGround;
	bool isW, isA, isD;

	// ref component
	Transform* tr;
	SpriteRenderer* sr;
	Rigidbody* rb;

public:
	// script component cycle
	void OnEnable() override;				// ������Ʈ Ȱ��ȭ ����
	void OnDisable() override;				// ������Ʈ ��Ȱ��ȭ ����
	void Awake() override;					// ������Ʈ�� ��� ������Ʈ�� ������ ���� ���� 1ȸ ȣ��
	void Start() override;					// Awake() ���� ���� 1ȸ ȣ��
	void Update() override;					// �� ������ �ݺ� ȣ��
	void FixedUpdate() override;			// ���� ������Ʈ 0.02f ���� �ݺ� ȣ��
	void OnDestroy() override;				// ������Ʈ or ������Ʈ �Ҹ� ����

	// trigger
	void OnTriggerEnter(ICollider* other) override;
	void OnTriggerStay(ICollider* other)  override;
	void OnTriggerExit(ICollider* other)  override;

	// collision
	void OnCollisionEnter(ICollider* other, const ContactInfo& contact)  override;
	void OnCollisionStay(ICollider* other, const ContactInfo& contact) override;	
	void OnCollisionExit(ICollider* other, const ContactInfo& contact)  override;

private:
	void InputCheak();
	void Movement();
	void Jump();
};

