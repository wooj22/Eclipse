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
	const float MAX_HP = 15;

	// stat
	float hp = MAX_HP;
	float speed = 25;
	float palyer_deceleration = 0.2;

	// flag
	bool isDie;
	bool isGoal;
	bool isAttacking = false;

	// attack
	enum AttackPattern {RoundShell, DiffusedShell, DropShell};
	int currentAttackIndex = RoundShell;
	float attackCoolTime = 5.0f;
	float attackDeltaTime;

	// attack repeat
	float attackRepeatCoolTime = 1.5f;
	float attackRepeatDeltaTime;

	// attack 1 data
	const int bulletCount = 16;

	// attack 2 data
	const int attack2_bulletCount = 10;
	const float spreadAngle = 60.0f; // ±30도

	// attack 3 data
	const int attack3_bulletCount = 10;
	const float map_maxY = 960;
	const float map_minX = -1000.0f;
	const float map_maxX = 1000;
	
	// start dealy
	float bossStartTime = 7.0f;
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
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;
	void OnTriggerStay(ICollider* other, const ContactInfo& contact)  override;
	void OnTriggerExit(ICollider* other, const ContactInfo& contact)  override;

private:
	// boss contoll func
	void Move();

	// boss attack func
	void AttackHandler();
	void Attack(int attackIndex);
	void Attack_RoundShell();
	void Attack_DiffusedShell();
	void Attack_DropShell();

	// boss hit
public: void TakeDamage(int damage);
private: 
	void Die();

	// 투명화 연출
	void OpacityDirecting();

public:
	float GetBossMaxHp() { return MAX_HP; }
	float GetBossHp() { return hp; }
	bool GetBossDie() { return isDie; }
};

