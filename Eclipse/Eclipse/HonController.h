#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Vector2.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
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

public:
	bool destroyPending = false;		// sound delay

protected:
	// hon stat
	int hp = 5;
	float size = 1;
	float descentSpeed = 90;						// 하강 speed
	float collisionSpeed = 1000;					// 충돌 밀림 speed
	const float collisionSpeedDefalut = 1000;		// 충돌 밀림 기본 speed

	// data
	float collisionMovingTime = 0.5f;		// 충돌 밀림 지속시간
	float pullMovingTime = 0.2f;			// Hon C 끌어당김 지속시간

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

	// hp alpha data
	float alphaData[4] = { 0, 0.5f, 0.75f, 1.f };

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
public:	SpriteRenderer* sr = nullptr;
public:	CircleCollider* collider = nullptr;
protected: AudioSource* audioSource = nullptr;

public:
	HonController();

	// direction
	void SetDirection(Vector2 dir);
	Vector2 Getdirection();

	// size
	void SetSize(float s);
	float GetSize();

	// collision move speed
	void SetSpeedUpRate(float r);

	// descent speed
	void SetDescentSpeed(float s);
	float GetSDescentpeed();

	// hp
	void SetHp(int h);
	int GetHp();
	void TakeDamage(int damage);
	void Die();

	// player attack
	virtual void TakeDamageByPlayer() {}

	// collision moving flag
	void CollisionEnd();
	void CollisionStart();

	// HonC 끌어당기는 함수
	void HonC_PullMe(Vector2 pos);

	// 플레이어 흡수
	void Absorption();
};

