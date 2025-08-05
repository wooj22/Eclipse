#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/Vector2.h"

/* [C] Darkness 어둠의 혼 컨트롤러 Script */
// 연쇄반응
// 1. A : 좌우로 튕김
// 2. B : 무시
// 3. C : View안의 모든 혼을 끌어당기고 삭제
// 4. D : 파괴

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

	// data								    
	float collisionMovingTime = 3.0f;		// 충돌 밀림 지속시간
	float pullMovingTime = 0.7f;			// Hon C 끌어당김 지속시간

	// controll
	Vector2 moveDirection = Vector2::zero;
	Vector2 descentDirection = Vector2::down;
	Vector2 pullDirection = Vector2::zero;
	bool isCollisionMoving = false;
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

	void SetHp(int h) { hp = h; }
	int GetHp() { return hp; }
	void TakeDamage()
	{
		hp--;
		if (hp <= 0) gameObject->Destroy();
	}

	// HonC 끌어당기는 함수
	void HonC_PullMe(Vector2 pos);
};

