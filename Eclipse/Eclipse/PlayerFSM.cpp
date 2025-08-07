#include "PlayerFSM.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/AnimatorController.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include "../Direct2D_EngineLib/WorldTextRenderer.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/Camera.h"

#include "../Direct2D_EngineLib/RaycastHit.h"
#include "../Direct2D_EngineLib/ColliderSystem.h"

#include "Dash_State.h"

#include "GameManager.h"
#include "PlayUI.h"
#include "Chat.h"
#include "HonController.h"

#include "PlayerAnimatorController.h"



// ������Ʈ Ȱ��ȭ ����
void PlayerFSM::OnEnable()
{

}

void PlayerFSM::Awake()
{
	transform = gameObject->GetComponent<Transform>();
	spriteRenderer = gameObject->GetComponent<SpriteRenderer>();
	rigidbody = gameObject->GetComponent<Rigidbody>();
	animatorController = gameObject->GetComponent<Animator>()->controller;

	playerAnimatorController = dynamic_cast<PlayerAnimatorController*>(animatorController);

	// [ FSM �ʱ�ȭ ]
	movementFSM = std::make_unique<MovementFSM>();
	movementFSM->Init(this);
}

void PlayerFSM::Start()
{
	// [ ��ų �ر� ] �׽�Ʈ ���ؼ� 
	GameManager::Get().honCount = 1000;

	GameManager::Get().AllSkillUnlock();
}

void PlayerFSM::Update()
{

	//D2D1_POINT_2F start = { 0, 0 };
	//D2D1_POINT_2F end = { 0, -240 };
	//RenderSystem::Get().DebugDrawLine(start, end, transform->GetScreenMatrix(), 2.0f);

	// ray 
	//ray.direction = { Vector2::down };
	//ray.origin = transform->GetWorldPosition() - Vector2(0, 120);
	//hit = ColliderSystem::Get().Raycast(ray, 300);

	//if (hit.collider)
	//{
	//	hit.point.y
	//	std::string debugStr = "[PlayerFSM] hit.collider = " + hit.collider->gameObject->tag + "\n";
	//	OutputDebugStringA(debugStr.c_str());
	//}


	InputSetting(); // input Ű�� Ȯ��

	// [ Q E ��ų ]
	UpdateSkillCooldowns(); 
	if (isQ) { TryUseAbsorb(); }
	if (isE) { TryUseRelease(); }
	if (isF) { GameManager::Get().g_playUI->PlayerInteraction(); }

	movementFSM->Update();

	MouseWorldPos = Camera::GetScreenToWorldPosition(Input::GetMouseScreenPosition());

	SpeedSetting(); // [ Speed Setting ]

	FlipXSetting(); // [ FlipX Setting - ���� �̵� ���� ���� ]

	UpdateDashCooldown(); // dash ��Ÿ�� ������Ʈ

	if (isAbsorbSkillActive) 
	{
		AttractionTargetHon(); // [ Q ��ų ���� ]: Ÿ�� ȥ�� �÷��̾� ������ �۾����鼭 �ٰ�����
		
		if(!playerAnimatorController->GetSkillAvailable()) playerAnimatorController->SetSkillAvailable(true);
	} 
	
	if (isSpeedDown)
	{
		speedDownTimer -= Time::GetDeltaTime();
		if (speedDownTimer <= 0.0f)
		{
			speedDownRate = 1.0f; // ���� �ӵ��� ����
			isSpeedDown = false;

			OutputDebugStringA("[PlayerFSM] �ӵ� ���� ����\n");
		}
	}
	
	// [ FSM ���� ] 
	//MovementStateBase* currentState = GetMovementFSM()->GetCurrentState();
	//if (currentState)
	//{
	//	std::string name = typeid(*currentState).name();  // ���� �̸� Ȯ��
	//	OutputDebugStringA(("���� ����: " + name + "\n").c_str());
	//}


}

void PlayerFSM::FixedUpdate()
{
	movementFSM->FixedUpdate();
}

void PlayerFSM::OnDestroy()
{

}

void PlayerFSM::InputSetting()
{
	inputX = Input::GetAxisHorizontal();
	inputY = Input::GetAxisVertical();

	isA = Input::GetKey('A');
	isD = Input::GetKey('D');
	isS = Input::GetKey('S');
	isQ = Input::GetKeyDown('Q');
	isE = Input::GetKeyDown('E');
	isShift = Input::GetKey(VK_SHIFT);
	isSpace = Input::GetKeyDown(VK_SPACE);

	isLButton = Input::GetKeyDown(VK_LBUTTON);
	isRButton = Input::GetKeyDown(VK_RBUTTON);

	isF = Input::GetKeyDown('F');
}

void PlayerFSM::FlipXSetting()
{
	if (!isBulletFliping)
	{
		if (abs(rigidbody->velocity.x) > 0.01f)   // ���� ���°� �ƴ� ���� ���� �ݿ�
		{
			// spriteRenderer->flipX = rigidbody->velocity.x < 0.0f;  // �������� �̵� ���̸� flip
			spriteRenderer->flipX = rigidbody->velocity.x > 0.0f;  // ���������� �̵� ���̸� flip
			lastFlipX = spriteRenderer->flipX;
		}
		else   spriteRenderer->flipX = lastFlipX;  // �ӵ��� ���� 0�̸� ���� ���� ����
	}
	else { spriteRenderer->flipX = isBulletFlipX; } // BulletTime_State ���� ������ ���� 
}

void PlayerFSM::SpeedSetting()
{
	// Dash ������ �� curSpeed ��� X
	if (movementFSM && movementFSM->IsInState<Dash_State>()) return;

	if (isA || isD)
	{
		curSpeed = (walkSpeed * GetMoveSpeedBonus()) * speedDownRate; // ��ų �رݿ� ���� �߰� �̵� �ӵ�
	}
	else curSpeed = 0;

	 //std::string debugStr = "[PlayerFSM] Current Speed: " + std::to_string(curSpeed) + "\n";
	 //OutputDebugStringA(debugStr.c_str());
}



// *-------------- [ Skill ] --------------*

// jump

void PlayerFSM::OnGround()
{
	canAttackAfterJump[JumpPhase::NormalJump] = true;
	canAttackAfterJump[JumpPhase::DoubleJump] = true;
	canAttackAfterJump[JumpPhase::WallJump] = true;
}

void PlayerFSM::OnJump(JumpPhase jumpType)
{
	// ���� ���� �ر� ���� 
	if (!GameManager::Get().CheckUnlock(SkillType::JumpAttackExtra))
	{
		// ���� false�� �ʱ�ȭ
		for (auto& pair : canAttackAfterJump) pair.second = false;
	}

	// �ش� ������ true�� ����
	canAttackAfterJump[jumpType] = true;
}

bool PlayerFSM::CanAttack()
{
	// ���� ������ ������ ����
	if (GetIsGround())	return true;

	// ���߿����� ���� ���� �� �ϳ��� true�� ���� ����
	for (const auto& pair : canAttackAfterJump)
	{
		if (pair.second) return true;
	}

	return false;
}

void PlayerFSM::OnAirAttack() // � ���� ���¿����� �����ߴٸ� �ش� �÷��׸� ����
{
	for (auto& pair : canAttackAfterJump)
	{
		if (pair.second)
		{
			pair.second = false;
			break; // �� ���� ����
		}
	}
}


// dash
void PlayerFSM::UpdateDashCooldown() // Dash ��Ÿ�� ������Ʈ
{
	if (dashCooldownTimer > 0.0f)
	{
		dashCooldownTimer -= Time::GetDeltaTime();  // ��Ÿ�� Ÿ�̸Ӹ� ���ҽ�Ŵ
	}
}

bool PlayerFSM::CanDash() const // ��ð� �������� üũ
{
	return dashCooldownTimer <= 0.0f;  // ��Ÿ���� �������� ��� ����
}

void PlayerFSM::ResetDashCooldown() // ��� �� ��Ÿ�� �ʱ�ȭ
{
	dashCooldownTimer = dashCooldown;
}

// [ Q E skill ]
void PlayerFSM::TryUseAbsorb() // [ ��� ] 
{
	if (!CanUseAbsorb()){ OutputDebugStringA("[Skill] Q ��� ���� - ��Ÿ�� �Ǵ� �̹� ����\n"); return; }

	targetHon = FindNearestSoulInRange(absorbRange); // ���� ���� ȥ ã�� 
	//std::string debugStr = "[PlayerFSM] Absorb Hunmon's tag = " + targetHon->tag + "\n";
	//OutputDebugStringA(debugStr.c_str());

	if (targetHon)
	{
		targetHon->GetComponent<HonController>()->Absorption(); // ��� ������ �� ȣ�� 

		isAbsorbSkillActive = true; // ȥ ������� ���� 
		hasAbsorbedSoul = true;
		isReleaseSkillAvailable = true;
		absorbCooldownTimer = GetSkillCooldown();

		std::string debugStr = "[PlayerFSM] Q ��ų ��Ÿ�� = " + std::to_string(absorbCooldownTimer) + "\n";
		OutputDebugStringA(debugStr.c_str());

		OutputDebugStringA("[Skill] Q ��� ���� - ��ȥ �����\n");
	}
	else
	{
		OutputDebugStringA("[Skill] Q ��� ���� - ���� �� ��ȥ ����\n");
	}
}

void PlayerFSM::TryUseRelease() // [ ���� ] 
{
	if (!CanUseRelease())
	{
		OutputDebugStringA("[Skill] E ���� ���� - ����� ��ȥ ����\n");
		return;
	}

	// Honmun Ž�� & ���� 
	int removedCount = 0;
	for (auto* obj : GameObject::FindAllWithTag("Hon"))
	{
		if (!obj || obj == targetHon) continue; // targetHon ����

		float dist = (obj->GetComponent<Transform>()->GetPosition() - transform->GetPosition()).Magnitude();
		if (dist <= releaseEffectRange)
		{
			obj->Destroy(); // ȥ ����
			removedCount++;
			GameManager::Get().honCount++;
		}
	}

	// ���� ����
	hasAbsorbedSoul = false;
	isReleaseSkillAvailable = false;

	// ����Ʈ �ߵ� 
	// HonReleaseEffect(); // ���� ����Ʈ, ������ 

	// �ִϸ��̼� ���� ��ȯ
	playerAnimatorController->SetSkillAvailable(false);

	std::string debugStr = "[Skill] E ���� ���� - " + std::to_string(removedCount) + "�� ȥ ���ŵ�\n";
	OutputDebugStringA(debugStr.c_str());
}

GameObject* PlayerFSM::FindNearestSoulInRange(float range)
{
	GameObject* closestSoul = nullptr;
	float closestDist = FLT_MAX;

	for (auto* obj : GameObject::FindAllWithTag("Hon"))
	{
		if( obj->name == "HonD" ) continue;

		float dist = (obj->GetComponent<Transform>()->GetPosition() - transform->GetPosition()).Magnitude();
		if (dist < range && dist < closestDist)
		{
			closestSoul = obj;
			closestDist = dist;
		}
	}
	return closestSoul;
}

void PlayerFSM::AttractionTargetHon()
{
	if (!targetHon) return;

	honTimer += Time::GetDeltaTime();

	// Ÿ�� ȥ - �÷��̾� ������ �ٰ�����
	Vector2 targetPosition = this->transform->GetPosition();
	Vector2 currentPosition = targetHon->GetComponent<Transform>()->GetPosition();

	float speed = 10.0f; // ȥ �̵� �ӵ� 
	Vector2 newPosition = Vector2::Lerp(currentPosition, targetPosition, speed * Time::GetDeltaTime());
	targetHon->GetComponent<Transform>()->SetPosition(newPosition);

	// ȥ ũ�� ���� �ӵ�
	float scaleSpeed = 8.0f; 
	Vector2 currentScale = targetHon->GetComponent<Transform>()->GetScale();

	Vector2 newScale = Vector2(
		currentScale.x - scaleSpeed * Time::GetDeltaTime(),
		currentScale.y - scaleSpeed * Time::GetDeltaTime()
	);

	// �ּ� ũ�� ���� (Scale�� 0 �̸� X)
	if (newScale.x < 0.1f) newScale.x = 0.1f;
	if (newScale.y < 0.1f) newScale.y = 0.1f;

	// ũ�� ������Ʈ
	targetHon->GetComponent<Transform>()->SetScale(newScale);

	float distanceToPlayer = (targetPosition - currentPosition).Magnitude(); 
	float removeDistanceThreshold = 10.0f;  
	if (distanceToPlayer < removeDistanceThreshold || honTimer >= honQLifetime)
	{
		targetHon->Destroy();  // ȥ ����
		isAbsorbSkillActive = false;
	}
}

void PlayerFSM::UpdateSkillCooldowns()
{
	if (absorbCooldownTimer > 0.0f) absorbCooldownTimer -= Time::GetDeltaTime();
}

bool PlayerFSM::CanUseAbsorb() const
{
	return absorbCooldownTimer <= 0.0f && !hasAbsorbedSoul;
}

bool PlayerFSM::CanUseRelease() const
{
	return hasAbsorbedSoul;
}

// *-------------- [ Collider ] --------------*

void PlayerFSM::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
{

}

void PlayerFSM::OnTriggerStay(ICollider* other, const ContactInfo& contact)
{
	// mo_dev
	if (other->gameObject->name == "NPC" && !GameManager::Get().g_playUI->ChatActiveCheck()
		&& !GameManager::Get().isWave && Input::GetKey('F'))
	{
		GameManager::Get().g_playUI->ChatSetActive(true);
	}
}

void PlayerFSM::OnTriggerExit(ICollider* other, const ContactInfo& contact)
{

}


void PlayerFSM::OnCollisionEnter(ICollider* other, const ContactInfo& contact)
{
	if (other->gameObject->name == "Ground" && contact.normal.y > 0.5)
	{
		// OutputDebugStringA("Ground�� �浹 �߽��ϴ�.\n");
		isGround = true;
	}
	else if (other->gameObject->name == "Wall")
	{
		// OutputDebugStringA("Wall�� �浹 �߽��ϴ�.\n");
		isWall = true;
		if (contact.normal.x == 1) { isWallLeft = true; }
		if (contact.normal.x == -1) { isWallRight = true; }
	}
}

void PlayerFSM::OnCollisionStay(ICollider* other, const ContactInfo& contact)
{

}

void PlayerFSM::OnCollisionExit(ICollider* other, const ContactInfo& contact)
{
	if (other->gameObject->name == "Ground" && contact.normal.y > 0.5)
	{
		// OutputDebugStringA("Ground ���������� .\n");
		isGround = false;
	}
	else if (other->gameObject->name == "Wall")
	{
		isWall = false;
		if (contact.normal.x == 1)   isWallLeft = false;
		if (contact.normal.x == -1)  isWallRight = false;
	}
}


// *-------------- [ GameManager - Skill ] ------------------*

float PlayerFSM::GetMoveSpeedBonus() const
{
	return GameManager::Get().GetSkillBonus(SkillType::MoveSpeedUp); 
}

float PlayerFSM::GetAttackRangeBonus() const
{
	return GameManager::Get().GetSkillBonus(SkillType::AttackRangeUp);
}

float PlayerFSM::GetSkillCooldown() const
{
	float baseCooldown = absorbCooldown; // �⺻ ��Ÿ��
	float cooldownReduction = GameManager::Get().GetSkillBonus(SkillType::SkillCooldownDown); 
	return (((0.0f) > (baseCooldown - cooldownReduction)) ? (0.0f) : (baseCooldown - cooldownReduction)); // std::max
} 