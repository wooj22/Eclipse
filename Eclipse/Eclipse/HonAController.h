#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/Vector2.h"

class Transform;
class CircleCollider;

class HonAController : public Script
{
private:
	// hon stat
	int hp = 3;
	float descentSpeed = 80;		// 하강 speed
	float collisionSpeed = 150;		// 충돌 밀림 speed
	float size = 1;
	float pushBackTime = 3.0f;		// 충돌시 pushBackTime 시간동안 이동

	// controll
	Vector2 direction = Vector2::zero;
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

	void SetDescentSpeed(float s) { descentSpeed = s; }
	float GetSDescentpeed() { return descentSpeed; }

	void SetCollisionState(bool b) { isCollisionMoving = b; }
};

