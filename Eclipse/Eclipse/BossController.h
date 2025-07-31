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
	
	// start dealy
	float bossStartTime = 5.0f;
	float bossStartDeltaTime;

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

