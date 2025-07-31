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
	bool isGoal;

	// attack
	enum AttackPattern {RoundShell, DiffusedShell, DropShell};
	AttackPattern currentAttack = DiffusedShell;
	float attackCoolTime = 7.0f;
	float attackDeltaTime;
	bool isAttackIng = false;

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
	void FixedUpdate() override;
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
};

