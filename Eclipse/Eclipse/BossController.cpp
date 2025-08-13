#include "BossController.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include "Bullet.h"
#include "GameManager.h"
#include "PlayerFSM.h"

/*-----------------  component life cycle  ----------------*/
void BossController::Awake()
{
	// component
	tr = gameObject->transform;
	sr = gameObject->GetComponent<SpriteRenderer>();
	rb = gameObject->GetComponent<Rigidbody>();
	collider = gameObject->GetComponent<CircleCollider>();

	bossFace = GameObject::Find("BossFace")->GetComponent<SpriteRenderer>();
	playerTr = GameObject::Find("Player")->GetComponent<Transform>();

	// resource
	auto texture1 = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Boss/Boss_Idle.png");
	idleFace = ResourceManager::Get().CreateSprite(texture1, "BossIdleFace");

	auto texture2 = ResourceManager::Get().CreateTexture2D("../Resource/Woo/Boss/Boss_Fight.png");
	attackFace = ResourceManager::Get().CreateSprite(texture2, "BossAttackFace");

	// face set
	bossFace->sprite = idleFace;
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
		if (isHit) HitEffect();

		// debug
		if (Input::GetKeyDown('0')) TakeDamage(5);
	}
	else
	{
		OpacityDirecting();
		if (sr->GetAlpha() <= 0) {
			bossFace->gameObject->Destroy();
			this->gameObject->Destroy();
		}
	}
}

void BossController::OnDestroy()
{

}


/*--------------------  boss function  ---------------------*/
void BossController::Move()
{
	if (!isAttacking && !isGoal)
	{
		tr->Translate(Vector2::down * speed * Time::GetDeltaTime());
	}
}

/*--------------------  boss attack  ---------------------*/
// Boss Attack Handler
void BossController::AttackHandler()
{
	if (!isAttacking)
	{
		// cooltime
		attackDeltaTime += Time::GetDeltaTime();

		// first attack
		if (attackDeltaTime >= attackCoolTime)
		{
			Attack(currentAttackIndex);
			isAttacking = true;

			// face
			bossFace->sprite = attackFace;

			// attack coolTime reset
			attackDeltaTime = 0;
		}
	}
	else
	{
		// cooltime
		attackDeltaTime += Time::GetDeltaTime();
		attackRepeatDeltaTime += Time::GetDeltaTime();

		// second attack
		if (attackRepeatDeltaTime >= attackRepeatCoolTime)
		{
			Attack(currentAttackIndex);

			// face
			bossFace->sprite = idleFace;

			// attack index set
			currentAttackIndex =
				currentAttackIndex + 1 > DropShell ? RoundShell : currentAttackIndex += 1;

			// attack repeat coolTime reset
			attackRepeatDeltaTime = 0;
			isAttacking = false;
		}
	}
}

void BossController::Attack(int attackIndex)
{
	switch (attackIndex)
	{
	case BossController::RoundShell:
		Attack_RoundShell();
		Camera::GetMainCamera()->Shake(7, 10, 0.7);
		break;
	case BossController::DiffusedShell:
		Attack_DiffusedShell();
		Camera::GetMainCamera()->Shake(10, 10, 0.7);
		break;
	case BossController::DropShell:
		Attack_DropShell();
		Camera::GetMainCamera()->Shake(12, 8, 3.5, 1.5);
		break;
	default:
		break;
	}
}

// 1. Attack - 원형탄
// 보스 중심에서 360도 전방향으로 16발의 탄막이 동일 각도로 퍼짐
void BossController::Attack_RoundShell()
{
	Vector2 center = tr->GetWorldPosition();
	const float angleStep = 360.0f / bulletCount;

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

// 2. Attack - 확산탄
// 플레이어가 있는 방향으로 10발의 탄을 퍼트림
void BossController::Attack_DiffusedShell()
{
	Vector2 bossPos = tr->GetWorldPosition();
	Vector2 playerPos = playerTr->GetWorldPosition();
	Vector2 baseDir = (playerPos - bossPos).Normalized();
	const float angleStep = spreadAngle / (attack2_bulletCount - 1);

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

// 3. Attack - 낙하탄
// 상단에서 가로 일렬로 무작위 탄 10발이 떨어짐
void BossController::Attack_DropShell()
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

/*--------------------  boss hit  ---------------------*/
void BossController::TakeDamage(int damage)
{
	// hit effect
	isHit = true;
	hitTimer = 0.f;
	blinkStep = 0;

	// damage
	hp -= damage;
	GameManager::Get().ChangeBossHp(hp / MAX_HP);
	if (hp <= 0)
	{
		hp = 0;
		Die();
	}
}

void BossController::HitEffect()
{
	hitTimer += Time::GetDeltaTime();

	if (hitTimer >= blinkInterval)
	{
		hitTimer -= blinkInterval;
		blinkStep++;

		if (blinkStep % 2 == 0) sr->SetAlpha(1.0f);
		else sr->SetAlpha(0.4f);	
	}

	// end
	if (blinkStep >= 6)
	{
		sr->SetAlpha(1.0f);
		isHit = false;
	}
}

void BossController::Die()
{
	isDie = true;
	collider->SetEnabled(false);

	// wave4 quest
	GameManager::Get().ChangeQuestCount(4);
}

// 투명화 연출
void BossController::OpacityDirecting()
{
	float currentAlpha = sr->GetAlpha();
	float changeSpeed = 2;

	if (currentAlpha < 0)
		currentAlpha = min(currentAlpha + changeSpeed * Time::GetDeltaTime(), 0);
	else if (currentAlpha > 0)
		currentAlpha = max(currentAlpha - changeSpeed * Time::GetDeltaTime(), 0);

	sr->SetAlpha(currentAlpha);
	bossFace->SetAlpha(currentAlpha);
}

/*-----------------  trigger event  -----------------*/ 
void BossController::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
{
	// [Player Collision]
	if (other->gameObject->name == "Player")
	{
		other->gameObject->GetComponent<PlayerFSM>()->SetSpeedDownRate(palyer_deceleration);
	}

	// [Game Over]
	if (other->gameObject->tag == "EndLine")
	{
		isGoal = true;
		isDie = true;
	}
}

void BossController::OnTriggerStay(ICollider* other, const ContactInfo& contact)
{

}

void BossController::OnTriggerExit(ICollider* other, const ContactInfo& contact)
{

}