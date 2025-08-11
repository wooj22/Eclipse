#pragma once
#include<map>

#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/ICollider.h"
#include "../Direct2D_EngineLib/RaycastHit.h"
#include "../Direct2D_EngineLib/ColliderSystem.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include "../Direct2D_EngineLib/AudioClip.h"
#include "../Direct2D_EngineLib/AudioSource.h"

// FSM 
#include "MovementFSM.h"
#include "ActionFSM.h"

#include "GameManager.h"



enum class JumpPhase
{
	// None,       // 공중에 안뜬 상태
	NormalJump,
	DoubleJump,
	WallJump,
	COUNT
};

class Transform;
class SpriteRenderer;
class AnimatorController;
class WorldTextRenderer;
class Rigidbody;
class PlayerAttackArea;
class PlayerAnimatorController;
class PlayerSkillEffect;


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
	float walkSpeed = 460.0f; // 400.0f;
	float dashSpeed = 0.0f;
	float jumpForce = 1450.0f; // 3000.0f;

	bool isSpeedDownRate = false;

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
	bool isA, isD, isS, isShift, isSpace, isQ, isE, isF; // moon_dev

	bool isLButton;      // 현재 눌림 상태
	bool isLButtonDown;  // 이번 프레임에 눌림
	bool isLButtonUp;    // 이번 프레임에 뗌

	bool isRButton;
	bool isRButtonDown;
	bool isRButtonUp;

	// ui
	bool isUIOn = false;

private:
	// component
	Transform* transform = nullptr;
	SpriteRenderer* spriteRenderer = nullptr;
	Rigidbody* rigidbody = nullptr;
	AudioSource* audioSource = nullptr;
	AnimatorController* animatorController = nullptr;
	
	PlayerAttackArea* playerAttackArea = nullptr;
	GameObject* playerAttackParent = nullptr;

	GameObject* targetHon = nullptr;

	PlayerAnimatorController* playerAnimatorController = nullptr;
	// SkillAnimatorController* skillAnimatorController = nullptr;
	PlayerSkillEffect* skillEffect = nullptr;

public:
	// [ Audio ]
	shared_ptr<AudioClip> SFX_Player_Jump = nullptr;
	shared_ptr<AudioClip> SFX_Player_Land = nullptr;
	shared_ptr<AudioClip> SFX_Player_Attack = nullptr;
	shared_ptr<AudioClip> SFX_Player_Dash = nullptr;
	shared_ptr<AudioClip> SFX_Player_Move1 = nullptr;
	shared_ptr<AudioClip> SFX_Player_Move2 = nullptr;
	shared_ptr<AudioClip> SFX_Player_Move3 = nullptr;
	shared_ptr<AudioClip> SFX_Player_Move4 = nullptr;


public:
	// [ FSM 변수 ] : GameManager 에서 해금된 상태 가져와서 각 상태에서 조건 적용 
	float holdTime = 0.0f;
	float timer = 0.0f;

	bool canDoubleJump = false;             // 다시 땅 밟기 전까지 더블점프는 한번만 가능 
	bool canHanging = false;				// 다시 땅 밟기 전까지 매달리기는 한번만 가능 
	bool canFallAttack = false;				// 다시 땅 밟기 전까지 공중에서 공격 가능 

 	bool isHolding = false;

	const float bulletTimeThreshold = 0.2f; // 불릿 타임 시작 
	const float bulletTimeDuration = 2.0f;  // 불릿 유지 시간 
	const float ignoreInputDuration = 1.5f; // 입력 무시

	const float defaultGravity = 230.0f;    // 기본 중력 
	const float fastFallGravity = 400.0f;   // 빠른 하강 시, 중력 
	const float fastFallForce = -3000.0f;
	bool didFastFall = false;   // 빠른 하강 1회 
		 
	Vector2 MouseWorldPos;					// 실시간 마우스 월드 좌표 

	// attack 
	float maxAttackDistance = 150.0f;       // 공격 시, 최대 이동 거리 
	float attackDesiredTime = 0.7f;         // 도달 시간 0.3f -> 0.7f

	// boss
	float speedDownTimer = 0.0f;     // 실제로 줄어드는 타이머
	float speedDownDuration = 1.0f;  // 속도 감소 지속 시간 (고정)
	float speedDownRate = 1.0f;      // 곱해질 속도 비율 
	bool isSpeedDown = false;

public:
	// getter
	bool GetIsGround() const { return isGround; }
	bool GetIsSpace() const { return isSpace; }
	bool GetisShift() const { return isShift; }   // 임시 대쉬
	bool GetIsA() const { return isA; }
	bool GetIsD() const { return isD; }
	bool GetIsS() const { return isS; }
	
	bool GetIsLButton() const { return isLButton; }
	bool GetIsLButtonDown() const { return isLButtonDown; }
	bool GetIsLButtonUp() const { return isLButtonUp; }

	bool GetIsRButton() const { return isRButton; }
	bool GetIsRButtonDown() const { return isRButtonDown; }
	bool GetIsRButtonUp() const { return isRButtonUp; }

	float GetJumpForce() const { return jumpForce; }
	float GetInputX() const { return inputX; }
	float GetCurSpeed() const { return curSpeed; }
	float GetWalkSpeed() const { return walkSpeed; }

	// 이동속도 감소
	void SetSpeedDownRate(float rate)
	{
		speedDownTimer = speedDownDuration; // 고정된 지속 시간으로 초기화
		speedDownRate = rate;               
		isSpeedDown = true;

		OutputDebugStringA("[PlayerFSM] 속도 감소 적용\n");
	}

	bool GetIsWall() const { return isWall; }
	bool GetIsWallLeft() const { return isWallLeft; }
	bool GetIsWallRight() const { return isWallRight; }

	bool GetLastFlipX() const { return lastFlipX; } // true 오른쪽, false 왼쪽 (리소스 방향 변경됨)
	void SetLastFlipX(bool isFlipX) { lastFlipX = isFlipX; } 
	bool GetisBulletFliping() const { return isBulletFliping; }
	void SetisBulletFliping(bool isBulletTime) { isBulletFliping = isBulletTime; }
	bool GetisBulletFlipX() const { return isBulletFlipX; } // true 왼쪽 
	void SetisBulletFlipX(bool isBulletX) { isBulletFlipX = isBulletX; }
	void SetInvincible(bool value) { isInvincible = value; } // 무적 상태 여부 
	bool GetIsInvincible() const { return isInvincible; }

	Rigidbody* GetRigidbody() const { return rigidbody; }
	AnimatorController* GetAnimatorController() const { return animatorController; }
    Transform* GetTransform() const { return transform; }
	SpriteRenderer* GetSpriteRenderer() const { return spriteRenderer; }
	AudioSource* GetAudioSource() const { return audioSource; }

	void SetPlayerAttackArea(PlayerAttackArea* obj) { playerAttackArea = obj; }
	PlayerAttackArea* GetPlayerAttackArea() const { return playerAttackArea; }
	void SetPlayerAttackParent(GameObject* obj) { playerAttackParent = obj; }
	GameObject* GetPlayerAttackParent() const { return playerAttackParent; }
	void SetPlayerSkillEffect(PlayerSkillEffect* effect) { skillEffect = effect; }
	PlayerSkillEffect* GetPlayerSkillEffect() const { return skillEffect; }

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
	float dashCooldown = 1.5f;         // 대시 쿨타임 (2초)
	float dashCooldownTimer = 0.0f;    // 대시 쿨타임 타이머

	// jump 
	void OnGround();
	void OnJump(JumpPhase jumpType);	// 해당 점프 타입만 true, 나머지 false
	bool CanAttack();					// 공격 가능 조건 판별
	void OnAirAttack();					// 공격 시 현재 가능한 점프 타입의 플래그 false

	bool isAttackIgnore = false; // 어택 일정시간 무시 

	// dash 
	void UpdateDashCooldown(); 
	bool CanDash() const;
	void ResetDashCooldown();

	// Q 흡수 
	bool isAbsorbSkillActive = false;
	float absorbCooldown = 10.0f;		// 기본 쿨타임 
	float absorbCooldownTimer = 0.0f;
	float absorbRange = 300.0f;			// 흡수 범위 
	bool hasAbsorbedSoul = false;       // 저장 여부

	float honTimer = 0.0f;				// 혼의 지속 시간 누적
	const float honQLifetime = 1.0f;		// 혼이 플레이어 따라오는 최대 지속 시간 (초)

	// E 방출 
	bool isReleaseSkillAvailable = false; // 흡수된 혼이 있는가? = 방출 가능한가? 
	float releaseEffectRange = 400.0f;

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

	// [ Q Skill ]
	void AttractionTargetHon();

public:
	// [ GameManager - Skill ] 

	float GetMoveSpeedBonus() const; // speed 
	float GetAttackRangeBonus() const; // attack 
	float GetSkillCooldown() const; // skill CoolTime 
};

