#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/Vector2.h"

class Transform;
class SpriteRenderer;
class Rigidbody;
class CircleCollider;

class HonDController : public Script
{

private:
	// hon stat
	float descentSpeed = 80;		// 하강 speed
	float collisionSpeed = 150;		// 충돌 밀림 speed
	float pushBackTime = 5.0f;		// 충돌시 pushBackTime 시간동안 이동

	// controll
	Vector2 direction = Vector2::zero;
	Vector2 descentDirection = Vector2::down;
	Vector2 pullDirection = Vector2::zero;
	bool isCollisionMoving = false;	// 충돌로 밀려지는 상태
	float pushBackDeltaTime;

	// player 
	Transform* playerTr;

	// ref component
	Transform* tr;
	CircleCollider* collider;

public:
	// script component cycle
	void Awake() override;
	void Update() override;

	// trigger event
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;

public:
	// func
	void SetDirection(Vector2 dir) { direction = dir; }
	Vector2 Getdirection() { return direction; }
};

