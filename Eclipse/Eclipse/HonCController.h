#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/Vector2.h"

/* [C] Darkness ¾îµÒÀÇ È¥ ÄÁÆ®·Ñ·¯ Script */
// ¿¬¼â¹ÝÀÀ
// 1. A : ÁÂ¿ì·Î Æ¨±è
// 2. B : ¹«½Ã
// 3. C : View¾ÈÀÇ ¸ðµç È¥À» ²ø¾î´ç±è
// 4. D : ÆÄ±«

class Transform;
class CircleCollider;

class HonCController : public Script
{
private:
	// hon stat
	int hp = 3;
	float size = 1;
	float descentSpeed = 80;				// ÇÏ°­ speed
	float collisionSpeed = 150;				// Ãæµ¹ ¹Ð¸² speed

	// data								    // TODO :: Ãæµ¹ CC »©°í ´Ù¸¥ Cµéµµ ´ç±â±â
	float collisionMovingTime = 3.0f;		// Ãæµ¹ ¹Ð¸² Áö¼Ó½Ã°£

	// controll
	Vector2 moveDirection = Vector2::zero;
	Vector2 descentDirection = Vector2::down;
	Vector2 pullDirection = Vector2::zero;
	bool isCollisionMoving = false;

	// delta			
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
	// get & set
	void SetDirection(Vector2 dir) { moveDirection = dir; }
	Vector2 Getdirection() { return moveDirection; }

	void SetSize(float s) { size = s;  gameObject->transform->SetScale(size, size); }
	float GetSize() { return size; }

	void SetDescentSpeed(float s) { descentSpeed = s; }
	float GetDescentSpeed() { return descentSpeed; }
};

