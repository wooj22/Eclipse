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
}

void BossController::Start()
{

}

void BossController::Update()
{
	Move();
	AttackHandler();


	if (Input::GetKeyDown(VK_SPACE))
	{
		Instantiate<Bullet>({0,-500});
	}
}

void BossController::FixedUpdate()
{
	
}

void BossController::OnDestroy()
{

}


/*--------------------  boss function  ---------------------*/
void BossController::Move()
{
	// 공격중이 아니고, 아직 목표에 도달 안했을 경우 하강
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
		// cur attack index set
		// RoundShell, DiffusedShell, DropShell
		currentAttack = RoundShell;

		// attack
		switch (currentAttack)
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

		attackDeltaTime = 0;
	}
}

// 1. Attack - 원형탄
void BossController::Attack_RoundShell()
{
	OutputDebugStringA("Boss : Attack_RoundShell !\n");
}

// 2. Attack - 확산탄
void BossController::Attack_DiffusedShell()
{
	OutputDebugStringA("Boss : Attack_DiffusedShell !\n");
}

// 3. Attack - 낙하탄
void BossController::Attack_DropShell()
{
	OutputDebugStringA("Boss : Attack_DropShell !\n");
}


/*-------------  trigger event  -------------*/ 
void BossController::OnTriggerEnter(ICollider* other)
{
	if (other->gameObject->tag == "Goal")
	{
		isGoal = true;
		// TODO :: GameManager 게임 오버 전달
	}
}

void BossController::OnTriggerStay(ICollider* other)
{

}

void BossController::OnTriggerExit(ICollider* other)
{

}