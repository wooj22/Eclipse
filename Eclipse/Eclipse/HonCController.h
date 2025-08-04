#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/Vector2.h"

class Transform;
class CircleCollider;

class HonCController : public Script
{
private:
	// hon stat
	int hp = 3;
	float size = 1;
	float descentSpeed = 80;				// 하강 speed
	float collisionSpeed = 150;				// 충돌 밀림 speed
	float collisionMovingTime = 5.0f;		// 충돌 밀림 지속시간

	// controll
	Vector2 moveDirection = Vector2::zero;
	Vector2 descentDirection = Vector2::down;
	Vector2 pullDirection = Vector2::zero;

	// flag, delta
	bool isCollisionMoving = false;			
	float collisionMovingDelta;

	// player 
	Transform* playerTr;

	// ref component
	Transform* tr;
	CircleCollider* collider;

public:
	// script component cycle
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	// trigger event
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;

public:
	// func
	void SetDirection(Vector2 dir) { moveDirection = dir; }
	Vector2 Getdirection() { return moveDirection; }

	void SetSize(float s) { size = s;  gameObject->transform->SetScale(size, size); }
	float GetSize() { return size; }

	void SetDescentSpeed(float s) { descentSpeed = s; }
	float GetDescentSpeed() { return descentSpeed; }
};

