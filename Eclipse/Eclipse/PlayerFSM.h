#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/ICollider.h"

// FSM 
#include "MovementFSM.h"
#include "ActionFSM.h"

class Transform;
class SpriteRenderer;
class AnimatorController;
class WorldTextRenderer;
class Rigidbody;
// class BoxCollider;
class PlayerAttackArea;

class PlayerFSM : public Script
{
private:
	// FSM 
	std::unique_ptr<MovementFSM> movementFSM;
	std::unique_ptr<ActionFSM> actionFSM;

public:
	MovementFSM* GetMovementFSM() { return movementFSM.get(); }
	ActionFSM* GetActionFSM() { return actionFSM.get(); }

private:
	// stat
	float curSpeed = 0;
	float walkSpeed = 280.f;
	float dashSpeed = 450.f;
	float jumpForce = 700.0f;

	float speedDownRate = 1.0;

	// int lastWallDir = 0;  // -1: 왼쪽, 1: 오른쪽, 0: 없음

	// move
	float inputX, inputY;
	bool isGround;
	bool lastFlipX = false;
	bool isBulletFliping = false;
	bool isBulletFlipX = false;

	bool isWallLeft = false;
	bool isWallRight = false;


	// key
	bool isA, isD, isS, isShift, isSpace, isLButton, isRButton; // moon_dev

public:
	// key
	bool isF; // mo_dev 

private:
	// ref component
	Transform* transform = nullptr;
	SpriteRenderer* spriteRenderer = nullptr;
	Rigidbody* rigidbody = nullptr;
	AnimatorController* animatorController = nullptr;
	// BoxCollider* boxCollider = nullptr;

	PlayerAttackArea* playerAttackArea = nullptr;
	GameObject* playerAttackParent = nullptr;

public:
	// [ FSM 변수 ]
	// GameManager 에서 해금된 상태 가져와서 각 상태에서 조건 해주기 
	float holdTime = 0.0f;
	float timer = 0.0f;
	bool isHolding = false;
	const float bulletTimeThreshold = 0.4f;
	const float bulletTimeDuration = 2.0f;  // 불릿 유지 시간 
	const float ignoreInputDuration = 1.5f; // 입력 무시
	const float defaultGravity = 100.0f;   // 기본 중력 
	const float fastFallGravity = 400.0f;   // 빠른 하강 시, 중력 
	Vector2 MouseWorldPos;

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

	// TODO : 매개변수로 감속율 받는 함수 (우정)
	// 감속률만 받도록 
	// 쿨타임 후, 원래 속도 복원 
	void SetSpeedDownRate(float speed) { speedDownRate = speed; }


	bool GetIsWallLeft() const { return isWallLeft; }
	bool GetIsWallRight() const { return isWallRight; }

	bool GetLastFlipX() const { return lastFlipX; } // true 왼쪽 
	void SetLastFlipX(bool isFlipX) { lastFlipX = isFlipX; } // true 왼쪽 
	bool GetisBulletFliping() const { return isBulletFliping; }
	void SetisBulletFliping(bool isBulletTime) { isBulletFliping = isBulletTime; }
	bool GetisBulletFlipX() const { return isBulletFlipX; } // true 왼쪽 
	void SetisBulletFlipX(bool isBulletX) { isBulletFlipX = isBulletX; }

	Rigidbody* GetRigidbody() const { return rigidbody; }
	AnimatorController* GetAnimatorController() const { return animatorController; }
    Transform* GetTransform() const { return transform; }
	void SetPlayerAttackArea(PlayerAttackArea* obj) { playerAttackArea = obj; }
	PlayerAttackArea* GetPlayerAttackArea() const { return playerAttackArea; }
	void SetPlayerAttackParent(GameObject* obj) { playerAttackParent = obj; }
	GameObject* GetPlayerAttackParent() const { return playerAttackParent; }

	// BoxCollider* GetBoxCollider() const { return boxCollider; }

	// void SetLastWallDir(int dir) { lastWallDir = dir; }


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
	float GetSpeed() { return curSpeed; }

private:
	void InputCheak();
};

