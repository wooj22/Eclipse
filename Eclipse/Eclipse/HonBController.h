#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/Vector2.h"

class Transform;
class CircleCollider;

class HonBController : public Script
{
private:
	// hon stat
	int hp = 3;
	float descentSpeed = 80;		// �ϰ� speed
	float collisionSpeed = 150;		// �浹 �и� speed
	float size = 1;
	float pushBackTime = 5.0f;		// �浹�� pushBackTime �ð����� �̵�

	// controll
	Vector2 direction = Vector2::zero;
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
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	// trigger event
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;

public:
	// func
	void SetDirection(Vector2 dir) { direction = dir; }
	Vector2 Getdirection() { return direction; }

	void SetSize(float s) { size = s;  gameObject->transform->SetScale(size, size); }
	float GetSize() { return size; }

	void SetDescentSpeed(float s) { descentSpeed = s; }
	float GetDescentSpeed() { return descentSpeed; }
};

