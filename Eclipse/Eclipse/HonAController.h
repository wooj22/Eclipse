#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/Vector2.h"

class Transform;
class SpriteRenderer;
class Rigidbody;
class CircleCollider;

class HonAController : public Script
{
private:
	// hon stat
	int hp = 3;
	float speed = 80;
	float collisionSpeed = 150;
	float size = 1;
	float pushBackTime = 5.0f;		// 충돌시 pushBackTime 시간동안 이동

	// controll
	Vector2 direction = Vector2::zero;
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
	void SetDirection(Vector2 dir){ direction = dir; }
	Vector2 Getdirection() { return direction; }

	void SetSize(float s){ size = s;  tr->SetScale(size, size); }
	float GetSize() { return size; }

	void SetSpeed(float s) { speed = s; }
	float GetSpeed() { return speed; }
};

