#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Vector2.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "GameManager.h"
#include "PlayerFSM.h"
#include "Attack_State.h"

/* Hon Type Enum */
enum class HonType
{
	A,B,C,D, NONE
};

/* Hon A, B, C, D Controller Base */
class HonController : public Script
{
public:
	HonType honType = HonType::NONE;

protected:
	// hon stat
	int hp = 3;
	float size = 1;
	float descentSpeed = 85;						// �ϰ� speed
	float collisionSpeed = 1000;					// �浹 �и� speed
	const float collisionSpeedDefalut = 1000;		// �浹 �и� �⺻ speed

	// data
	float collisionMovingTime = 0.5f;		// �浹 �и� ���ӽð�
	float pullMovingTime = 0.15f;			// Hon C ������ ���ӽð�

	// controll
	Vector2 moveDirection = Vector2::zero;		// colliison move dir
	Vector2 descentDirection = Vector2::down;	// �ϰ� dir
	Vector2 pullDirection = Vector2::zero;		// Hon C ������ dir
	bool isCollisionMoving = false;				// �浹�� �з����� �ִ� ����
	bool isPullMoving = false;				    // C���� ������� �ִ� ����
	bool isAbsorption = false;					// �÷��̾�� ����ǰ� �ִ� ���� (D�� �Ⱦ����� �÷��̾��� ȣ�� ���ϼ��� ����)

	// delta
	float collisionMovingDelta = 0.0f;
	float pullMovingDelta = 0.0f;

	// player 
	Transform* playerTr;
	float palyer_deceleration = 0.2;

	// ref component
	Transform* tr = nullptr;
	CircleCollider* collider = nullptr;

public:
	// direction
	void SetDirection(Vector2 dir) { moveDirection = dir; }
	Vector2 Getdirection() { return moveDirection; }

	// size
	void SetSize(float s) 
	{
		size = s;  
		if(tr) tr->SetScale(size, size); 
	}
	float GetSize() { return size; }

	// collision move speed
	void SetSpeedUpRate(float r) { collisionSpeed = collisionSpeedDefalut * r; }

	// descent speed
	void SetDescentSpeed(float s) { descentSpeed = s; }
	float GetSDescentpeed() { return descentSpeed; }

	// hp
	void SetHp(int h) { hp = h; }
	int GetHp() { return hp; }
	void TakeDamage()
	{
		hp--;
		if (hp <= 0) 
		{
			GameManager::Get().ChangeQuestCount(1);		// wave1 quest
			gameObject->Destroy();
		}
	}

	// player attack
	virtual void TakeDamageByPlayer() {}

	// collision moving flag
	void CollisionEnd() { isCollisionMoving = false; collisionMovingDelta = 0; }
	void CollisionStart() { isCollisionMoving = true; collisionMovingDelta = 0; }

	// HonC ������� �Լ�
	void HonC_PullMe(Vector2 pos)
	{
		pullDirection = (pos - tr->GetWorldPosition()).Normalized();
		isPullMoving = true;
	}

	// �÷��̾� ���
	void Absorption() 
	{ 
		isAbsorption = true;
		if (collider) collider->SetEnabled(false);
	}
};

