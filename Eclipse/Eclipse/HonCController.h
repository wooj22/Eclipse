#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/Vector2.h"

/* [C] Darkness ����� ȥ ��Ʈ�ѷ� Script */
// �������
// 1. A : �¿�� ƨ��
// 2. B : ����
// 3. C : View���� ��� ȥ�� ������� ����
// 4. D : �ı�

class Transform;
class CircleCollider;

class HonCController : public Script
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
	Vector2 moveDirection = Vector2::zero;
	Vector2 descentDirection = Vector2::down;
	Vector2 pullDirection = Vector2::zero;
	bool isCollisionMoving = false;
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

	// HonC ������� �Լ�
	void HonC_PullMe(Vector2 pos);
};

