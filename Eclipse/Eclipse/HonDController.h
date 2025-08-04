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
	float descentSpeed = 80;				// �ϰ� speed
	float collisionSpeed = 150;				// �浹 �и� speed
	float collisionMovingTime = 3.0f;		// �浹 �и� ���ӽð�
	float pullMovingTime = 1.0f;			// Hon C ������ ���ӽð�

	// controll
	Vector2 moveDirection = Vector2::zero;		// colliison move dir		
	Vector2 descentDirection = Vector2::down;	// �ϰ� dir
	Vector2 pullDirection = Vector2::zero;		// Hon C ������ dir

	// flag
	bool isCollisionMoving = false;				// �浹�� �з����� �ִ� ����
	bool isPullMoving = false;				    // C���� ������� �ִ� ����

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

	// HonC ������� �Լ�
	void HonC_PullMe(Vector2 pos);
};

