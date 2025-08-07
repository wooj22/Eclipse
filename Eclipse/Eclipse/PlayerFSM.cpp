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

	playerAnimatorController = dynamic_cast<PlayerAnimatorController*>(animatorController);

	// [ FSM 초기화 ]
	movementFSM = std::make_unique<MovementFSM>();
	movementFSM->Init(this);
}

void PlayerFSM::Start()
{
	// [ 스킬 해금 ] 테스트 위해서 
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


	InputSetting(); // input 키값 확인

	// [ Q E 스킬 ]
	UpdateSkillCooldowns(); 
	if (isQ) { TryUseAbsorb(); }
	if (isE) { TryUseRelease(); }
	if (isF) { GameManager::Get().g_playUI->PlayerInteraction(); }

	movementFSM->Update();

	MouseWorldPos = Camera::GetScreenToWorldPosition(Input::GetMouseScreenPosition());

	SpeedSetting(); // [ Speed Setting ]

	FlipXSetting(); // [ FlipX Setting - 실제 이동 방향 기준 ]

	UpdateDashCooldown(); // dash 쿨타임 업데이트

	if (isAbsorbSkillActive) 
	{
		AttractionTargetHon(); // [ Q 스킬 상태 ]: 타겟 혼이 플레이어 쪽으로 작아지면서 다가오기
		
		if(!playerAnimatorController->GetSkillAvailable()) playerAnimatorController->SetSkillAvailable(true);
	} 
	
	if (isSpeedDown)
	{
		speedDownTimer -= Time::GetDeltaTime();
		if (speedDownTimer <= 0.0f)
		{
			speedDownRate = 1.0f; // 원래 속도로 복귀
			isSpeedDown = false;

			OutputDebugStringA("[PlayerFSM] 속도 감소 해제\n");
		}
	}
	
	// [ FSM 상태 ] 
	//MovementStateBase* currentState = GetMovementFSM()->GetCurrentState();
	//if (currentState)
	//{
	//	std::string name = typeid(*currentState).name();  // 상태 이름 확인
	//	OutputDebugStringA(("현재 상태: " + name + "\n").c_str());
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
		if (abs(rigidbody->velocity.x) > 0.01f)   // 정지 상태가 아닐 때만 방향 반영
		{
			// spriteRenderer->flipX = rigidbody->velocity.x < 0.0f;  // 왼쪽으로 이동 중이면 flip
			spriteRenderer->flipX = rigidbody->velocity.x > 0.0f;  // 오른쪽으로 이동 중이면 flip
			lastFlipX = spriteRenderer->flipX;
		}
		else   spriteRenderer->flipX = lastFlipX;  // 속도가 거의 0이면 이전 방향 유지
	}
	else { spriteRenderer->flipX = isBulletFlipX; } // BulletTime_State 에서 변수값 조정 
}

void PlayerFSM::SpeedSetting()
{
	// Dash 상태일 땐 curSpeed 계산 X
	if (movementFSM && movementFSM->IsInState<Dash_State>()) return;

	if (isA || isD)
	{
		curSpeed = (walkSpeed * GetMoveSpeedBonus()) * speedDownRate; // 스킬 해금에 따른 추가 이동 속도
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
	// 점프 공격 해금 유무 
	if (!GameManager::Get().CheckUnlock(SkillType::JumpAttackExtra))
	{
		// 전부 false로 초기화
		for (auto& pair : canAttackAfterJump) pair.second = false;
	}

	// 해당 점프만 true로 설정
	canAttackAfterJump[jumpType] = true;
}

bool PlayerFSM::CanAttack()
{
	// 땅에 있으면 언제나 가능
	if (GetIsGround())	return true;

	// 공중에서는 점프 종류 중 하나라도 true면 공격 가능
	for (const auto& pair : canAttackAfterJump)
	{
		if (pair.second) return true;
	}

	return false;
}

void PlayerFSM::OnAirAttack() // 어떤 점프 상태에서든 공격했다면 해당 플래그를 꺼줌
{
	for (auto& pair : canAttackAfterJump)
	{
		if (pair.second)
		{
			pair.second = false;
			break; // 한 번만 차감
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

// [ Q E skill ]
void PlayerFSM::TryUseAbsorb() // [ 흡수 ] 
{
	if (!CanUseAbsorb()){ OutputDebugStringA("[Skill] Q 흡수 실패 - 쿨타임 또는 이미 보유\n"); return; }

	targetHon = FindNearestSoulInRange(absorbRange); // 범위 내의 혼 찾기 
	//std::string debugStr = "[PlayerFSM] Absorb Hunmon's tag = " + targetHon->tag + "\n";
	//OutputDebugStringA(debugStr.c_str());

	if (targetHon)
	{
		targetHon->GetComponent<HonController>()->Absorption(); // 흡수 시작할 때 호출 

		isAbsorbSkillActive = true; // 혼 끌어당기기 시작 
		hasAbsorbedSoul = true;
		isReleaseSkillAvailable = true;
		absorbCooldownTimer = GetSkillCooldown();

		std::string debugStr = "[PlayerFSM] Q 스킬 쿨타임 = " + std::to_string(absorbCooldownTimer) + "\n";
		OutputDebugStringA(debugStr.c_str());

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

	// Honmun 탐색 & 제거 
	int removedCount = 0;
	for (auto* obj : GameObject::FindAllWithTag("Hon"))
	{
		if (!obj || obj == targetHon) continue; // targetHon 제외

		float dist = (obj->GetComponent<Transform>()->GetPosition() - transform->GetPosition()).Magnitude();
		if (dist <= releaseEffectRange)
		{
			obj->Destroy(); // 혼 제거
			removedCount++;
			GameManager::Get().honCount++;
		}
	}

	// 상태 리셋
	hasAbsorbedSoul = false;
	isReleaseSkillAvailable = false;

	// 이펙트 발동 
	// HonReleaseEffect(); // 범위 이펙트, 데미지 

	// 애니메이션 상태 변환
	playerAnimatorController->SetSkillAvailable(false);

	std::string debugStr = "[Skill] E 방출 성공 - " + std::to_string(removedCount) + "개 혼 제거됨\n";
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

	// 타겟 혼 - 플레이어 쪽으로 다가오기
	Vector2 targetPosition = this->transform->GetPosition();
	Vector2 currentPosition = targetHon->GetComponent<Transform>()->GetPosition();

	float speed = 10.0f; // 혼 이동 속도 
	Vector2 newPosition = Vector2::Lerp(currentPosition, targetPosition, speed * Time::GetDeltaTime());
	targetHon->GetComponent<Transform>()->SetPosition(newPosition);

	// 혼 크기 감소 속도
	float scaleSpeed = 8.0f; 
	Vector2 currentScale = targetHon->GetComponent<Transform>()->GetScale();

	Vector2 newScale = Vector2(
		currentScale.x - scaleSpeed * Time::GetDeltaTime(),
		currentScale.y - scaleSpeed * Time::GetDeltaTime()
	);

	// 최소 크기 제한 (Scale이 0 미만 X)
	if (newScale.x < 0.1f) newScale.x = 0.1f;
	if (newScale.y < 0.1f) newScale.y = 0.1f;

	// 크기 업데이트
	targetHon->GetComponent<Transform>()->SetScale(newScale);

	float distanceToPlayer = (targetPosition - currentPosition).Magnitude(); 
	float removeDistanceThreshold = 10.0f;  
	if (distanceToPlayer < removeDistanceThreshold || honTimer >= honQLifetime)
	{
		targetHon->Destroy();  // 혼 제거
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
	float baseCooldown = absorbCooldown; // 기본 쿨타임
	float cooldownReduction = GameManager::Get().GetSkillBonus(SkillType::SkillCooldownDown); 
	return (((0.0f) > (baseCooldown - cooldownReduction)) ? (0.0f) : (baseCooldown - cooldownReduction)); // std::max
} 