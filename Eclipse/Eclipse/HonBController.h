#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/Vector2.h"

/* [B] Umbra �׸����� ȥ ��Ʈ�ѷ� Script */
// �������
// 1. A : �浹 �ݴ� �������� ƨ��
// 2. B : �� ������Ʈ�� 2���� �и� (ũ�� -30%, �ϰ��ӵ� +20%)
// 3. C : ����
// 4. D : �ı� 

class Transform;
class CircleCollider;

class HonBController : public Script
{
private:
	// hon stat
	int hp = 3;
	float size = 1;
	float descentSpeed = 80;				// �ϰ� speed
	float collisionSpeed = 150;				// �浹 �и� speed
	
	// data
	float collisionMovingTime = 3.0f;		// �浹 �и� ���ӽð�
	float pullMovingTime = 0.7f;			// Hon C ������ ���ӽð�

	// controll
	Vector2 moveDirection = Vector2::zero;	    // colliison move dir
	Vector2 descentDirection = Vector2::down;	// �ϰ� dir
	Vector2 pullDirection = Vector2::zero;	    // Hon C ������ dir
	bool isCollisionMoving = false;				// �浹�� �з����� ����
	bool isPullMoving = false;				    // C���� ������� �ִ� ����

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
	// get & set
	void SetDirection(Vector2 dir) { moveDirection = dir; }
	Vector2 Getdirection() { return moveDirection; }

	void SetSize(float s) { size = s;  gameObject->transform->SetScale(size, size); }
	float GetSize() { return size; }

	void SetDescentSpeed(float s) { descentSpeed = s; }
	float GetDescentSpeed() { return descentSpeed; }

	void SetHp(int h) { hp = h; }
	int GetHp() { return hp; }

	// HonC ������� �Լ�
	void HonC_PullMe(Vector2 pos);
};

