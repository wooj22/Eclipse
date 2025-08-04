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
	float descentSpeed = 80;				// 하강 speed
	float collisionSpeed = 150;				// 충돌 밀림 speed
	float collisionMovingTime = 3.0f;		// 충돌 밀림 지속시간
	float pullMovingTime = 1.0f;			// Hon C 끌어당김 지속시간

	// controll
	Vector2 moveDirection = Vector2::zero;		// colliison move dir		
	Vector2 descentDirection = Vector2::down;	// 하강 dir
	Vector2 pullDirection = Vector2::zero;		// Hon C 끌어당김 dir

	// flag
	bool isCollisionMoving = false;				// 충돌로 밀려지고 있는 상태
	bool isPullMoving = false;				    // C에게 당겨지고 있는 상태

	// delta
	float collisionMovingDelta;
	float pullMovingDelta;

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
	void SetDirection(Vector2 dir) { moveDirection = dir; }
	Vector2 Getdirection() { return moveDirection; }

	// HonC 끌어당기는 함수
	void HonC_PullMe(Vector2 pos);
};

