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
	float descentSpeed = 80;		// �ϰ� speed
	float collisionSpeed = 150;		// �浹 �и� speed
	float pushBackTime = 5.0f;		// �浹�� pushBackTime �ð����� �̵�

	// controll
	Vector2 direction = Vector2::zero;
	Vector2 descentDirection = Vector2::down;
	Vector2 pullDirection = Vector2::zero;
	bool isCollisionMoving = false;	// �浹�� �з����� ����
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

