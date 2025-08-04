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

#include "GameManager.h"
#include "PlayUI.h"
#include "Chat.h"
#include "Dash_State.h"


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


	// [ FSM �ʱ�ȭ ]
	movementFSM = std::make_unique<MovementFSM>();
	movementFSM->Init(this);
}

void PlayerFSM::Start()
{
	// [ ��ų �ر� ] �׽�Ʈ ���ؼ� 
	// for (int i = 0; i < 3; ++i) 
	GameManager::Get().honCount = 1000;

	GameManager::Get().LevelUpSkill(SkillType::KnockbackDistanceUp); 
	GameManager::Get().LevelUpSkill(SkillType::KnockbackDistanceUp);
	GameManager::Get().LevelUpSkill(SkillType::KnockbackDistanceUp);
	GameManager::Get().LevelUpSkill(SkillType::DoubleJump);
	GameManager::Get().LevelUpSkill(SkillType::WallJump);

	GameManager::Get().LevelUpSkill(SkillType::SkillCooldownDown);
	GameManager::Get().LevelUpSkill(SkillType::SkillCooldownDown);
	GameManager::Get().LevelUpSkill(SkillType::JumpAttackExtra);
	GameManager::Get().LevelUpSkill(SkillType::FastFall);

	GameManager::Get().LevelUpSkill(SkillType::MoveSpeedUp);
	GameManager::Get().LevelUpSkill(SkillType::MoveSpeedUp);
	GameManager::Get().LevelUpSkill(SkillType::MoveSpeedUp);
	GameManager::Get().LevelUpSkill(SkillType::AttackRangeUp);
	GameManager::Get().LevelUpSkill(SkillType::AttackRangeUp);
	GameManager::Get().LevelUpSkill(SkillType::AttackRangeUp);
	GameManager::Get().LevelUpSkill(SkillType::Dash);
}

void PlayerFSM::Update()
{
	InputSetting(); // input Ű�� Ȯ��

	// [ Q E ��ų ]
	UpdateSkillCooldowns(); 
	if (isQ) { TryUseAbsorb(); }
	if (isE) { TryUseRelease(); }


	movementFSM->Update();

	MouseWorldPos = Camera::GetScreenToWorldPosition(Input::GetMouseScreenPosition());

	SpeedSetting(); // [ Speed Setting ]

	FlipXSetting(); // [ FlipX Setting - ���� �̵� ���� ���� ]

	UpdateDashCooldown(); // dash ��Ÿ�� ������Ʈ

	// [ FSM ���� ] 
	//MovementStateBase* currentState = playerFSM->GetMovementFSM()->GetCurrentState();
	//if (currentState)
	//{
	//	std::string name = typeid(*currentState).name();  // ���� �̸� Ȯ��
	//	OutputDebugStringA(("���� ����: " + name + "\n").c_str());
	//}


	// [ mo_dev ] 
	//if (isF)
	//{
	//	if (GameManager::Get().g_playUI->ChatActiveCheck())
	//	{
	//		if (!GameManager::Get().g_playUI->chat_Text->GetComponent<Chat>()->GetFinished())
	//			GameManager::Get().g_playUI->chat_Text->GetComponent<Chat>()->AddChatCount();
	//	}
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

	isF = Input::GetKey('F');
}

void PlayerFSM::FlipXSetting()
{
	if (!isBulletFliping)
	{
		if (abs(rigidbody->velocity.x) > 0.01f)   // ���� ���°� �ƴ� ���� ���� �ݿ�
		{
			// spriteRenderer->flipX = rigidbody->velocity.x < 0.0f;  // �������� �̵� ���̸� flip
			spriteRenderer->flipX = rigidbody->velocity.x > 0.0f;  // �������� �̵� ���̸� flip
			lastFlipX = spriteRenderer->flipX;
		}
		else   spriteRenderer->flipX = lastFlipX;  // �ӵ��� ���� 0�̸� ���� ���� ����
	}
	else   spriteRenderer->flipX = isBulletFlipX;  // BulletTime_State ���� ������ ����

}

void PlayerFSM::SpeedSetting()
{
	// Dash ������ �� curSpeed ��� X
	if (movementFSM && movementFSM->IsInState<Dash_State>()) return;

	if (isA || isD)
	{
		curSpeed = (walkSpeed + GetMoveSpeedBonus()) * speedDownRate; // ��ų �رݿ� ���� �߰� �̵� �ӵ�
	}
	else curSpeed = 0;

	// std::string debugStr = "[PlayerFSM] Current Speed: " + std::to_string(curSpeed) + "\n";
	// OutputDebugStringA(debugStr.c_str());
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
	// ���� �� ���� ���� ���� ����
	if (!GameManager::Get().CheckUnlock(SkillType::JumpAttackExtra))
	{
		if (jumpType == JumpPhase::NormalJump)	canAttackAfterJump[jumpType] = true;
		else									canAttackAfterJump[jumpType] = false;
	}
	else
	{
		canAttackAfterJump[jumpType] = true;
	}
}

bool PlayerFSM::CanAttack()
{
	for (auto it = canAttackAfterJump.begin(); it != canAttackAfterJump.end(); ++it)
	{
		if (it->second)  return true;
	}
	return false;
}

void PlayerFSM::UseAttack()
{
	for (auto it = canAttackAfterJump.begin(); it != canAttackAfterJump.end(); ++it)
	{
		if (it->second)
		{
			it->second = false;
			break; // �� ���� ��Ȱ��ȭ
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


// speed 
float PlayerFSM::GetMoveSpeedBonus() const 
{
	static const float speedBonusTable[] = { 0.0f, 50.0f, 150.0f, 250.0f }; // 0, 1, 3, 5, 
	int level = GameManager::Get().skillTree.at(SkillType::MoveSpeedUp).unlockLevel;

	// ���� ó��
	if (level < 0) level = 0; if (level > 3) level = 3;

	return speedBonusTable[level];
}

// attack
float PlayerFSM::GetAttackRangeBonus() const
{
	int level = GameManager::Get().skillTree[SkillType::AttackRangeUp].unlockLevel;
	switch (level)
	{
	case 1: return 50.0f;   // 0.5f;
	case 2: return 100.0f;  // 1.0f;
	case 3: return 150.0f;  // 1.5f;
	default: return 0.0f;
	}
}

// Q E skill 
void PlayerFSM::TryUseAbsorb() // [ ��� ] 
{
	if (!CanUseAbsorb()){ OutputDebugStringA("[Skill] Q ��� ���� - ��Ÿ�� �Ǵ� �̹� ����\n"); return; }

	GameObject* soul = FindNearestSoulInRange(absorbRange); // ���� ���� ȥ ã�� 
	//std::string debugStr = "[PlayerFSM] Absorb Hunmon's tag = " + soul->tag + "\n";
	//OutputDebugStringA(debugStr.c_str());

	if (soul)
	{
		soul->Destroy(); // ���(����)
		hasAbsorbedSoul = true;
		isReleaseSkillAvailable = true;
		absorbCooldownTimer = absorbCooldown;
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

	// 1. Honmun Ž�� �� ���� : �ϴ� ���� �� �����Ұ���! ���� �������� ���� �ʿ� 
	int removedCount = 0;
	for (auto* obj : GameObject::FindAll("Honmun"))
	{
		if (!obj) continue;

		float dist = (obj->GetComponent<Transform>()->GetPosition() - transform->GetPosition()).Magnitude();
		if (dist <= releaseEffectRange)
		{
			obj->Destroy(); // ȥ ����
			removedCount++;
		}
	}

	// 2. ���� ����
	hasAbsorbedSoul = false;
	isReleaseSkillAvailable = false;

	// 3. ����Ʈ �ߵ� 
	// PerformReleaseEffect(); // ���� ����Ʈ, ������ 

	std::string debugStr = "[Skill] E ���� ���� - " + std::to_string(removedCount) + "�� ȥ ���ŵ�\n";
	OutputDebugStringA(debugStr.c_str());
}

GameObject* PlayerFSM::FindNearestSoulInRange(float range)
{
	GameObject* closestSoul = nullptr;
	float closestDist = FLT_MAX;

	for (auto* obj : GameObject::FindAll("Honmun"))
	{
		float dist = (obj->GetComponent<Transform>()->GetPosition() - transform->GetPosition()).Magnitude();
		if (dist < range && dist < closestDist)
		{
			closestSoul = obj;
			closestDist = dist;
		}
	}
	return closestSoul;
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
	//if (other->gameObject->name == "NPC" && !GameManager::Get().g_playUI->ChatActiveCheck()
	//	&& !GameManager::Get().isWave && Input::GetKey('F'))
	//{
	//	GameManager::Get().g_playUI->ChatSetActive(true);
	//}
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