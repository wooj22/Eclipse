#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/ICollider.h"

/* 엔진 기능 추가 테스트용 */

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
	void OnEnable() override;				// 컴포넌트 활성화 시점
	void OnDisable() override;				// 컴포넌트 비활성화 시점
	void Awake() override;					// 오브젝트의 모든 컴포넌트가 생성된 직후 시점 1회 호출
	void Start() override;					// Awake() 이후 시점 1회 호출
	void Update() override;					// 매 프레임 반복 호출
	void FixedUpdate() override;			// 물리 업데이트 0.02f 보장 반복 호출
	void OnDestroy() override;				// 컴포넌트 or 오브젝트 소멸 시점

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

