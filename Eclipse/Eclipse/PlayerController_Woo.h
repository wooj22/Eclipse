#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/ICollider.h"

/* 엔진 기능 추가 테스트용 */

class Transform;
class SpriteRenderer;
class Rigidbody;

class PlayerController_Woo : public Script
{
private:
	// stat
	float speed = 400;
	float jumpForce = 3000.0f;

	// control
	float inputX, inputY;
	bool isGround;
	bool isW, isA, isD;

	// ref component
	Transform* tr;
	SpriteRenderer* sr;
	Rigidbody* rb;

public:
	// script component cycle
	void OnEnable() override;
	void OnDisable() override;
	void Awake() override;
	void Start() override;
	void Update() override;	
	void FixedUpdate() override;
	void OnDestroy() override;

	// trigger
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;
	void OnTriggerStay(ICollider* other, const ContactInfo& contact)  override;
	void OnTriggerExit(ICollider* other, const ContactInfo& contact)  override;

	// collision
	void OnCollisionEnter(ICollider* other, const ContactInfo& contact)  override;
	void OnCollisionStay(ICollider* other, const ContactInfo& contact) override;	
	void OnCollisionExit(ICollider* other, const ContactInfo& contact)  override;

private:
	void InputCheak();
	void Movement();
	void Jump();
};

