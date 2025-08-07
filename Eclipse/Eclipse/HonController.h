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
	float descentSpeed = 85;						// 하강 speed
	float collisionSpeed = 1000;					// 충돌 밀림 speed
	const float collisionSpeedDefalut = 1000;		// 충돌 밀림 기본 speed

	// data
	float collisionMovingTime = 0.5f;		// 충돌 밀림 지속시간
	float pullMovingTime = 0.15f;			// Hon C 끌어당김 지속시간

	// controll
	Vector2 moveDirection = Vector2::zero;		// colliison move dir
	Vector2 descentDirection = Vector2::down;	// 하강 dir
	Vector2 pullDirection = Vector2::zero;		// Hon C 끌어당김 dir
	bool isCollisionMoving = false;				// 충돌로 밀려지고 있는 상태
	bool isPullMoving = false;				    // C에게 당겨지고 있는 상태
	bool isAbsorption = false;					// 플레이어에게 흡수되고 있는 상태 (D는 안쓰지만 플레이어의 호출 통일성을 위해)

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

	// HonC 끌어당기는 함수
	void HonC_PullMe(Vector2 pos)
	{
		pullDirection = (pos - tr->GetWorldPosition()).Normalized();
		isPullMoving = true;
	}

	// 플레이어 흡수
	void Absorption() 
	{ 
		isAbsorption = true;
		if (collider) collider->SetEnabled(false);
	}
};

