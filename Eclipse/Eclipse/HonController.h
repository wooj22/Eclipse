#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Vector2.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/AudioClip.h"
#include "../Direct2D_EngineLib/AudioSource.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
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
	float is2A = false;
	float isCollisionD = false;

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

	// asset
	shared_ptr<AudioClip> SFX_HonCollision = nullptr;
	shared_ptr<AudioClip> SFX_HonDestroy = nullptr;
	shared_ptr<AudioClip> SFX_HonSplit = nullptr;
	shared_ptr<AudioClip> SFX_HonMerge = nullptr;

	// player 
	Transform* playerTr = nullptr;
	float palyer_deceleration = 0.2;

	// ref component
	Transform* tr = nullptr;
	SpriteRenderer* sr = nullptr;
	CircleCollider* collider = nullptr;
	AudioSource* audioSource = nullptr;

public:
	HonController()
	{
		SFX_HonCollision = ResourceManager::Get().CreateAudioClip("../Resource/Woo/Sound/s_Crash.wav");
		SFX_HonDestroy = ResourceManager::Get().CreateAudioClip("../Resource/Woo/Sound/s_Destroy.wav");
		SFX_HonSplit = ResourceManager::Get().CreateAudioClip("../Resource/Woo/Sound/s_Split.wav");
		SFX_HonMerge = ResourceManager::Get().CreateAudioClip("../Resource/Woo/Sound/s_Merge.wav");
	}

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
	void TakeDamage(int damage)
	{
		hp -= damage;
		if (hp <= 0) 
		{
			Die();
		}

		// reset
		isCollisionD = false;
	}

	void Die()
	{
		// wave1 quest
		GameManager::Get().ChangeQuestCount(1);

		// hon score
		if (honType == HonType::A)
		{
			if (isCollisionD)  GameManager::Get().ChangeHonCount(-1);
			else if (is2A) GameManager::Get().ChangeHonCount(3);
			else GameManager::Get().ChangeHonCount(1);
		}
		else if (honType == HonType::B)
		{
			if (isCollisionD)  GameManager::Get().ChangeHonCount(-1);
			else GameManager::Get().ChangeHonCount(1);
		}
		else if (honType == HonType::C)
		{
			if (isCollisionD)  GameManager::Get().ChangeHonCount(-1);
			else GameManager::Get().ChangeHonCount(1);
		}
		else if (honType == HonType::D)
		{
			GameManager::Get().ChangeHonCount(-1);
		}

		sr->SetEnabled(false);
		collider->SetEnabled(false);

		// sound
		audioSource->SetClip(SFX_HonDestroy);
		audioSource->PlayOneShot();

		// sound dealy
		// destroy
		gameObject->Destroy();
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

