#pragma once
#include "../Direct2D_EngineLib/Script.h"

/*---------------------*/
/* Boss Controller.cs */
/*-------------------*/

class Transform;
class SpriteRenderer;
class Rigidbody;
class CircleCollider;

class BossController : public Script
{
private:
	// data
	const float MAX_HP = 100;

	// stat
	float hp = MAX_HP;
	float speed = 20;
	float palyer_deceleration = 0.5;

	// flag
	bool isDie;
	bool isGoal;
	bool isAttackIng = false;

	// attack
	enum AttackPattern {RoundShell, DiffusedShell, DropShell};
	int currentAttackIndex = RoundShell;
	float attackCoolTime = 3.0f;
	float attackDeltaTime;

	// attack 1 data
	const int bulletCount = 16;

	// attack 2 data
	const int attack2_bulletCount = 10;
	const float spreadAngle = 60.0f; // ¡¾30µµ

	// attack 3 data
	const int attack3_bulletCount = 10;
	const float map_maxY = 960;
	const float map_minX = -1000.0f;
	const float map_maxX = 1000;
	
	// start dealy
	float bossStartTime = 5.0f;
	float bossStartDeltaTime;

	// player 
	Transform* playerTr;

	// ref component
	Transform* tr;
	SpriteRenderer* sr;
	Rigidbody* rb;
	CircleCollider* collider;

public:
	// script component cycle
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	// trigger event
	void OnTriggerEnter(ICollider* other) override;
	void OnTriggerStay(ICollider* other)  override;
	void OnTriggerExit(ICollider* other)  override;

private:
	// boss contoll func
	void Move();


	// boss attack func
	void AttackHandler();
	void Attack_RoundShell();
	void Attack_DiffusedShell();
	void Attack_DropShell();


	// boss hit
public: void TakeDamage(int damage);
private: void Die();

};

