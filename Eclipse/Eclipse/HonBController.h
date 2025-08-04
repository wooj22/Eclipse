#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/Vector2.h"

class Transform;
class CircleCollider;

class HonBController : public Script
{
private:
	// hon stat
	int hp = 3;								// HP
	float descentSpeed = 80;				// 하강 speed
	float collisionSpeed = 150;				// 충돌 밀림 speed
	float size = 1;							// Scale
	float collisionMovingTime = 5.0f;		// 충돌 밀림 지속시간
	float pullMovingTime = 1.0f;			// Hon C 끌어당김 지속시간

	// controll
	Vector2 moveDirection = Vector2::zero;	    // colliison move dir
	Vector2 descentDirection = Vector2::down;	// 하강 dir
	Vector2 pullDirection = Vector2::zero;	    // Hon C 끌어당김 dir

	// flag
	bool isCollisionMoving = false;				// 충돌로 밀려지는 상태
	bool isPullMoving = false;				    // C에게 당겨지고 있는 상태

	// delta
	float pushBackDeltaTime = 0.0f;
	float pullMovingDelta = 0.0f;

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

	// HonC 끌어당기는 함수
	void HonC_PullMe(Vector2 pos);
};

