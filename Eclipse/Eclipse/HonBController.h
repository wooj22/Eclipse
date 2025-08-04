#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/Vector2.h"

class Transform;
class SpriteRenderer;
class Rigidbody;
class CircleCollider;

class HonBController : public Script
{
private:
	// hon stat
	int hp = 3;
	float descentSpeed = 80;
	float collisionSpeed = 150;
	float size = 1;
	float pushBackTime = 5.0f;		// 충돌시 pushBackTime 시간동안 이동

	// controll
	Vector2 direction = Vector2::down;
	bool isPlayerCollision = false;
	float pushBackDeltaTime;

	// player 
	Transform* playerTr;

	// ref component
	Transform* tr;
	SpriteRenderer* sr;
	Rigidbody* rb;
	CircleCollider* collider;

public:
	// script component cycle
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	// trigger event
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;
	void OnTriggerStay(ICollider* other, const ContactInfo& contact)  override;
	void OnTriggerExit(ICollider* other, const ContactInfo& contact)  override;

public:
	// func
	void SetDirection(Vector2 dir) { direction = dir; }
	Vector2 Getdirection() { return direction; }

	void SetSize(float s) { size = s;  gameObject->transform->SetScale(size, size); }
	float GetSize() { return size; }

	void SetDescentSpeed(float s) { descentSpeed = s; }
	float GetDescentSpeed() { return descentSpeed; }
};

