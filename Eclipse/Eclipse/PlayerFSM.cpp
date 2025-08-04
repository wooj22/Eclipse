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


// 컴포넌트 활성화 시점
void PlayerFSM::OnEnable()
{

}

void PlayerFSM::Awake()
{
	transform = gameObject->GetComponent<Transform>();
	spriteRenderer = gameObject->GetComponent<SpriteRenderer>();
	rigidbody = gameObject->GetComponent<Rigidbody>();
	animatorController = gameObject->GetComponent<Animator>()->controller;


	// [ FSM 초기화 ]
	movementFSM = std::make_unique<MovementFSM>();
	movementFSM->Init(this);
}

void PlayerFSM::Start()
{
	// [ 스킬 해금 ] 테스트 위해서 
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
	InputSetting(); // input 키값 확인

	// [ Q E 스킬 ]
	UpdateSkillCooldowns(); 
	if (isQ) { TryUseAbsorb(); }
	if (isE) { TryUseRelease(); }


	movementFSM->Update();

	MouseWorldPos = Camera::GetScreenToWorldPosition(Input::GetMouseScreenPosition());

	SpeedSetting(); // [ Speed Setting ]

	FlipXSetting(); // [ FlipX Setting - 실제 이동 방향 기준 ]

	UpdateDashCooldown(); // dash 쿨타임 업데이트

	// [ FSM 상태 ] 
	//MovementStateBase* currentState = playerFSM->GetMovementFSM()->GetCurrentState();
	//if (currentState)
	//{
	//	std::string name = typeid(*currentState).name();  // 상태 이름 확인
	//	OutputDebugStringA(("현재 상태: " + name + "\n").c_str());
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
		if (abs(rigidbody->velocity.x) > 0.01f)   // 정지 상태가 아닐 때만 방향 반영
		{
			// spriteRenderer->flipX = rigidbody->velocity.x < 0.0f;  // 왼쪽으로 이동 중이면 flip
			spriteRenderer->flipX = rigidbody->velocity.x > 0.0f;  // 왼쪽으로 이동 중이면 flip
			lastFlipX = spriteRenderer->flipX;
		}
		else   spriteRenderer->flipX = lastFlipX;  // 속도가 거의 0이면 이전 방향 유지
	}
	else   spriteRenderer->flipX = isBulletFlipX;  // BulletTime_State 에서 변수값 조정

}

void PlayerFSM::SpeedSetting()
{
	// Dash 상태일 땐 curSpeed 계산 X
	if (movementFSM && movementFSM->IsInState<Dash_State>()) return;

	if (isA || isD)
	{
		curSpeed = (walkSpeed + GetMoveSpeedBonus()) * speedDownRate; // 스킬 해금에 따른 추가 이동 속도
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
	// 점프 시 공격 가능 여부 설정
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
			break; // 한 번만 비활성화
		}
	}
}

// dash
void PlayerFSM::UpdateDashCooldown() // Dash 쿨타임 업데이트
{
	if (dashCooldownTimer > 0.0f)
	{
		dashCooldownTimer -= Time::GetDeltaTime();  // 쿨타임 타이머를 감소시킴
	}
}

bool PlayerFSM::CanDash() const // 대시가 가능한지 체크
{
	return dashCooldownTimer <= 0.0f;  // 쿨타임이 끝났으면 대시 가능
}

void PlayerFSM::ResetDashCooldown() // 대시 후 쿨타임 초기화
{
	dashCooldownTimer = dashCooldown;
}


// speed 
float PlayerFSM::GetMoveSpeedBonus() const 
{
	static const float speedBonusTable[] = { 0.0f, 50.0f, 150.0f, 250.0f }; // 0, 1, 3, 5, 
	int level = GameManager::Get().skillTree.at(SkillType::MoveSpeedUp).unlockLevel;

	// 안전 처리
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
void PlayerFSM::TryUseAbsorb() // [ 흡수 ] 
{
	if (!CanUseAbsorb()){ OutputDebugStringA("[Skill] Q 흡수 실패 - 쿨타임 또는 이미 보유\n"); return; }

	GameObject* soul = FindNearestSoulInRange(absorbRange); // 범위 내의 혼 찾기 
	//std::string debugStr = "[PlayerFSM] Absorb Hunmon's tag = " + soul->tag + "\n";
	//OutputDebugStringA(debugStr.c_str());

	if (soul)
	{
		soul->Destroy(); // 흡수(제거)
		hasAbsorbedSoul = true;
		isReleaseSkillAvailable = true;
		absorbCooldownTimer = absorbCooldown;
		OutputDebugStringA("[Skill] Q 흡수 성공 - 영혼 저장됨\n");
	}
	else
	{
		OutputDebugStringA("[Skill] Q 흡수 실패 - 범위 내 영혼 없음\n");
	}
}

void PlayerFSM::TryUseRelease() // [ 방출 ] 
{
	if (!CanUseRelease())
	{
		OutputDebugStringA("[Skill] E 방출 실패 - 저장된 영혼 없음\n");
		return;
	}

	// 1. Honmun 탐색 및 제거 : 일단 냅다 다 삭제할게유! 추후 점수제로 변경 필요 
	int removedCount = 0;
	for (auto* obj : GameObject::FindAll("Honmun"))
	{
		if (!obj) continue;

		float dist = (obj->GetComponent<Transform>()->GetPosition() - transform->GetPosition()).Magnitude();
		if (dist <= releaseEffectRange)
		{
			obj->Destroy(); // 혼 제거
			removedCount++;
		}
	}

	// 2. 상태 리셋
	hasAbsorbedSoul = false;
	isReleaseSkillAvailable = false;

	// 3. 이펙트 발동 
	// PerformReleaseEffect(); // 범위 이펙트, 데미지 

	std::string debugStr = "[Skill] E 방출 성공 - " + std::to_string(removedCount) + "개 혼 제거됨\n";
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
		// OutputDebugStringA("Ground과 충돌 했습니다.\n");
		isGround = true;
	}
	else if (other->gameObject->name == "Wall")
	{
		// OutputDebugStringA("Wall과 충돌 했습니다.\n");
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
		// OutputDebugStringA("Ground 빠져나갔음 .\n");
		isGround = false;
	}
	else if (other->gameObject->name == "Wall")
	{
		isWall = false;
		if (contact.normal.x == 1)   isWallLeft = false;
		if (contact.normal.x == -1)  isWallRight = false;
	}
}