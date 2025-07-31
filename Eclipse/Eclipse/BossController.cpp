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
// 보스 중심에서 360도 전방향으로 16발의 탄막이 동일 각도로 퍼짐 * 2회
void BossController::Attack_RoundShell()
{
	Vector2 center = tr->GetWorldPosition();
	const float angleStep = 360.0f / bulletCount;

	for (int repeat = 0; repeat < 2; ++repeat)
	{
		for (int i = 0; i < bulletCount; ++i)
		{
			float angleDeg = i * angleStep;
			float angleRad = angleDeg * 3.14159265f / 180.0f;

			Vector2 dir = { cosf(angleRad), sinf(angleRad) };

			GameObject* bullet = Instantiate<Bullet>(center);
			BulletController* bc = bullet->GetComponent<BulletController>();

			bc->SetDirection(dir);
			bc->SetSpeed(250);
		}
	}
}

// 2. Attack - 확산탄
// 플레이어가 있는 방향으로 10발의 탄을 퍼트림 * 2회
void BossController::Attack_DiffusedShell()
{
	Vector2 bossPos = tr->GetWorldPosition();
	Vector2 playerPos = playerTr->GetWorldPosition();
	Vector2 baseDir = (playerPos - bossPos).Normalized();
	const float angleStep = spreadAngle / (attack2_bulletCount - 1);

	for (int repeat = 0; repeat < 2; ++repeat)
	{
		for (int i = 0; i < attack2_bulletCount; ++i)
		{
			float angleOffset = -spreadAngle / 2 + i * angleStep;
			float angleRad = angleOffset * 3.14159265f / 180.0f;

			// 회전 행렬 방식으로 baseDir 회전
			float cosA = cosf(angleRad);
			float sinA = sinf(angleRad);
			Vector2 rotated = {
				baseDir.x * cosA - baseDir.y * sinA,
				baseDir.x * sinA + baseDir.y * cosA
			};

			GameObject* bullet = Instantiate<Bullet>(bossPos);
			BulletController* bc = bullet->GetComponent<BulletController>();

			bc->SetDirection(rotated.Normalized());
			bc->SetSpeed(350);
		}
	}
}

// 3. Attack - 낙하탄
// 상단에서 가로 일렬로 무작위 탄 10발이 떨어짐 * 2회
void BossController::Attack_DropShell()
{
	for (int repeat = 0; repeat < 2; ++repeat)
	{
		for (int i = 0; i < attack3_bulletCount; ++i)
		{
			float randX = map_minX + static_cast<float>(rand()) / RAND_MAX * (map_maxX - map_minX);
			Vector2 spawnPos = { randX, map_maxY };

			GameObject* bullet = Instantiate<Bullet>(spawnPos);
			BulletController* bc = bullet->GetComponent<BulletController>();

			bc->SetDirection(Vector2::down);
			bc->SetSpeed(500);
		}
	}
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

void BossController::OnTriggerStay(ICollider* other)
{

}

void BossController::OnTriggerExit(ICollider* other)
{

}