#pragma once
#include<map>

#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/ICollider.h"

// FSM 
#include "MovementFSM.h"
#include "ActionFSM.h"

#include "GameManager.h"

enum class JumpPhase
{
	None,       // 공중에 안뜬 상태
	NormalJump,
	DoubleJump,
	WallJump
};

class Transform;
class SpriteRenderer;
class AnimatorController;
class WorldTextRenderer;
class Rigidbody;
class PlayerAttackArea;

class PlayerFSM : public Script
{
private:
	// FSM 
	std::unique_ptr<MovementFSM> movementFSM;
	std::unique_ptr<ActionFSM> actionFSM;

private:
	// Jump - skill 
	std::map<JumpPhase, bool> canAttackAfterJump;

public:
	MovementFSM* GetMovementFSM() { return movementFSM.get(); }
	ActionFSM* GetActionFSM() { return actionFSM.get(); }

private:
	// stat
	float curSpeed = 0;
	float walkSpeed = 280.0f;
	float dashSpeed = 450.0f;
	float jumpForce = 700.0f;

	float speedDownRate = 1.0; 

	bool isInvincible = false;	// dash 무적 상태 

	// move
	float inputX, inputY; // x : 왼쪽 -1 
	bool isGround;
	bool lastFlipX = false;
	bool isBulletFliping = false;
	bool isBulletFlipX = false;

	bool isWallLeft = false;
	bool isWallRight = false;
	bool isWall = false;


	// key
	bool isA, isD, isS, isShift, isSpace, isLButton, isRButton, isQ, isE; // moon_dev

public:
	// key
	bool isF; // mo_dev 

private:
	// ref component
	Transform* transform = nullptr;
	SpriteRenderer* spriteRenderer = nullptr;
	Rigidbody* rigidbody = nullptr;
	AnimatorController* animatorController = nullptr;

	PlayerAttackArea* playerAttackArea = nullptr;
	GameObject* playerAttackParent = nullptr;

public:
	// [ FSM 변수 ] : GameManager 에서 해금된 상태 가져와서 각 상태에서 조건 적용 
	float holdTime = 0.0f;
	float timer = 0.0f;

	bool canDoubleJump = false;             // 다시 땅 밟기 전까지 더블점프는 한번만 가능 
	bool isHolding = false;

	const float bulletTimeThreshold = 0.4f;
	const float bulletTimeDuration = 2.0f;  // 불릿 유지 시간 
	const float ignoreInputDuration = 1.5f; // 입력 무시
	const float defaultGravity = 100.0f;    // 기본 중력 
	const float fastFallGravity = 400.0f;   // 빠른 하강 시, 중력 

	Vector2 MouseWorldPos;					// 실시간 마우스 월드 좌표 

	// attack 
	float maxAttackDistance = 300.0f;       // 공격 시, 최대 이동 거리
	float attackDesiredTime = 0.3f;         // 도달 시간 0.3f


public:
	// getter
	bool GetIsGround() const { return isGround; }
	bool GetIsSpace() const { return isSpace; }
	bool GetisShift() const { return isShift; }   // 임시 대쉬
	bool GetIsA() const { return isA; }
	bool GetIsD() const { return isD; }
	bool GetIsS() const { return isS; }
	bool GetIsLButton() const { return isLButton; }
	bool GetIsRButton() const { return isRButton; }

	float GetJumpForce() const { return jumpForce; }
	float GetInputX() const { return inputX; }
	float GetCurSpeed() const { return curSpeed; }
	float GetWalkSpeed() const { return walkSpeed; }
	// float GetSpeed() const { return walkSpeed + GetMoveSpeedBonus(); }

	// TODO : 매개변수로 감속율 받는 함수 (우정)
	// 감속률만 받도록.  쿨타임 후, 원래 속도 복원 
	void SetSpeedDownRate(float speed) { speedDownRate = speed; }

	bool GetIsWall() const { return isWall; }
	bool GetIsWallLeft() const { return isWallLeft; }
	bool GetIsWallRight() const { return isWallRight; }

	bool GetLastFlipX() const { return lastFlipX; } // true 왼쪽 -> true 오른쪽, false 왼쪽 (리소스 방향 변경됨)
	void SetLastFlipX(bool isFlipX) { lastFlipX = isFlipX; } // true 왼쪽 
	bool GetisBulletFliping() const { return isBulletFliping; }
	void SetisBulletFliping(bool isBulletTime) { isBulletFliping = isBulletTime; }
	bool GetisBulletFlipX() const { return isBulletFlipX; } // true 왼쪽 
	void SetisBulletFlipX(bool isBulletX) { isBulletFlipX = isBulletX; }
	void SetInvincible(bool value) { isInvincible = value; } // 무적 상태 여부 
	bool GetIsInvincible() const { return isInvincible; }

	Rigidbody* GetRigidbody() const { return rigidbody; }
	AnimatorController* GetAnimatorController() const { return animatorController; }
    Transform* GetTransform() const { return transform; }
	void SetPlayerAttackArea(PlayerAttackArea* obj) { playerAttackArea = obj; }
	PlayerAttackArea* GetPlayerAttackArea() const { return playerAttackArea; }
	void SetPlayerAttackParent(GameObject* obj) { playerAttackParent = obj; }
	GameObject* GetPlayerAttackParent() const { return playerAttackParent; }


public:
	// script component cycle
	void OnEnable() override;				// 컴포넌트 활성화 시점
	void Awake() override;					// 오브젝트의 모든 컴포넌트가 생성된 직후 시점 1회 호출
	void Start() override;					// Awake() 이후 시점 1회 호출
	void Update() override;					// 매 프레임 반복 호출
	void FixedUpdate() override;			// 물리 업데이트 0.02f 보장 반복 호출
	void OnDestroy() override;				// 컴포넌트 or 오브젝트 소멸 시점

	// trigger
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;
	void OnTriggerStay(ICollider* other, const ContactInfo& contact)  override;
	void OnTriggerExit(ICollider* other, const ContactInfo& contact)  override;

	// collision
	void OnCollisionEnter(ICollider* other, const ContactInfo& contact)  override;
	void OnCollisionStay(ICollider* other, const ContactInfo& contact) override;
	void OnCollisionExit(ICollider* other, const ContactInfo& contact)  override;



public: 
	// [ skill ]
	bool canAttack_Unlock = true;          // 기본 공격 횟수 (해금 이후 공격 횟수는 각 State에서 관리)
	int airAttackCount = 0;

	// dash 
	float dashCooldown = 2.0f;         // 대시 쿨타임 (2초)
	float dashCooldownTimer = 0.0f;    // 대시 쿨타임 타이머

	// jump 
	void OnGround();
	void OnJump(JumpPhase jumpType);
	bool CanAttack();
	void UseAttack();

	// speed 
	float GetMoveSpeedBonus() const;

	// attack 
	float GetAttackRangeBonus() const;

	// dash 
	void UpdateDashCooldown(); 
	bool CanDash() const;
	void ResetDashCooldown();

	// Q 흡수 
	bool isAbsorbSkillActive = false;
	float absorbCooldown = 5.0f;		// 쿨타임 
	float absorbCooldownTimer = 0.0f;
	float absorbRange = 300.0f;			// 흡수 범위 
	bool hasAbsorbedSoul = false;       // 저장 여부

	// E 방출 
	bool isReleaseSkillAvailable = false;
	float releaseEffectRange = 400.0f;
	// float releaseDamage = 3.0f; // 혼 점수제에 따른 데미지 

	void UpdateSkillCooldowns();
	bool CanUseAbsorb() const;
	bool CanUseRelease() const;

	void TryUseAbsorb();
	void TryUseRelease();
	GameObject* FindNearestSoulInRange(float range);

private:
	// [ FSM setting ] 
	void InputSetting();
	void SpeedSetting();
	void FlipXSetting();
};

