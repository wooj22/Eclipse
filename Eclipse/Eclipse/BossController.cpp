#include "BossController.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"
#include "Bullet.h"

/*-----------------  component life cycle  ----------------*/
void BossController::Awake()
{
	tr = gameObject->transform;
	sr = gameObject->GetComponent<SpriteRenderer>();
	rb = gameObject->GetComponent<Rigidbody>();
	collider = gameObject->GetComponent<CircleCollider>();

	playerTr = GameObject::FindWithTag("Player_Woo")->GetComponent<Transform>();
}

void BossController::Start()
{

}

void BossController::Update()
{
	if (!isDie)
	{
		Move();
		AttackHandler();
	}

	// test :: player -> take damage
	if (Input::GetKeyDown('X'))
	{
		TakeDamage(10);
	}
}

void BossController::OnDestroy()
{

}


/*--------------------  boss function  ---------------------*/
void BossController::Move()
{
	if (!isAttackIng && !isGoal)
	{
		tr->Translate(Vector2::down * speed * Time::GetDeltaTime());
	}
}

/*--------------------  boss attack  ---------------------*/
// Boss Attack Handler
void BossController::AttackHandler()
{
	// cooltime
	attackDeltaTime += Time::GetDeltaTime();
	if (attackDeltaTime >= attackCoolTime)
	{
		// attack
		switch (currentAttackIndex)
		{
		case BossController::RoundShell:
			Attack_RoundShell();
			break;
		case BossController::DiffusedShell:
			Attack_DiffusedShell();
			break;
		case BossController::DropShell:
			Attack_DropShell();
			break;
		default:
			break;
		}

		// attack index, coolTime set
		currentAttackIndex = 
			currentAttackIndex + 1 > DropShell ? RoundShell : currentAttackIndex += 1;
		attackDeltaTime = 0;
	}
}

// 1. Attack - 원형탄
void BossController::Attack_RoundShell()
{
	Instantiate<Bullet>({ -100,-500 });
	OutputDebugStringA("Boss : Attack_RoundShell !\n");
}

// 2. Attack - 확산탄
void BossController::Attack_DiffusedShell()
{
	Instantiate<Bullet>({ 0,-500 });
	OutputDebugStringA("Boss : Attack_DiffusedShell !\n");
}

// 3. Attack - 낙하탄
void BossController::Attack_DropShell()
{
	Instantiate<Bullet>({ 100,-500 });
	OutputDebugStringA("Boss : Attack_DropShell !\n");
}


/*--------------------  boss hit  ---------------------*/
void BossController::TakeDamage(int damage)
{
	hp -= damage;
	if (hp < 0)
	{
		hp = 0;
		Die();
	}
}

void BossController::Die()
{
	isDie = true;
	this->gameObject->Destroy();
	// TODO :: GameManager 게임 성공 전달
	// TODO :: die animation? or 연출 or Destroy
}

/*-----------------  trigger event  -----------------*/ 
void BossController::OnTriggerEnter(ICollider* other)
{
	if (other->gameObject->tag == "Goal")
	{
		isGoal = true;
		// TODO :: GameManager 게임 오버 전달
	}
}