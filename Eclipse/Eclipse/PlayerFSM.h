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

	// int lastWallDir = 0;  // -1: ����, 1: ������, 0: ����

	// move
	float inputX, inputY;
	bool isGround;
	bool lastFlipX = false;
	bool isBulletFliping = false;
	bool isBulletFlipX = false;

	bool isWallLeft = false;
	bool isWallRight = false;


	// key
	bool isA, isD, isS, isShift, isSpace, isLButton, isRButton;

	// ref component
	Transform* transform = nullptr;
	SpriteRenderer* spriteRenderer = nullptr;
	Rigidbody* rigidbody = nullptr;
	AnimatorController* animatorController = nullptr;
	// BoxCollider* boxCollider = nullptr;

public:
	// [ FSM ���� ]
	// GameManager ���� �رݵ� ���� �����ͼ� �� ���¿��� ���� ���ֱ� 
	float holdTime = 0.0f;
	float timer = 0.0f;
	bool isHolding = false;
	const float bulletTimeThreshold = 0.4f;
	const float bulletTimeDuration = 2.0f;  // �Ҹ� ���� �ð� 
	const float ignoreInputDuration = 1.5f; // �Է� ����
	const float defaultGravity = 100.0f;   // �⺻ �߷� 
	const float fastFallGravity = 400.0f;   // ���� �ϰ� ��, �߷� 
	Vector2 MouseWorldPos;


public:
	// getter
	bool GetIsGround() const { return isGround; }
	bool GetIsSpace() const { return isSpace; }
	bool GetisShift() const { return isShift; }   // �ӽ� �뽬
	bool GetIsA() const { return isA; }
	bool GetIsD() const { return isD; }
	bool GetIsS() const { return isS; }
	bool GetIsLButton() const { return isLButton; }
	bool GetIsRButton() const { return isRButton; }

	float GetJumpForce() const { return jumpForce; }
	float GetInputX() const { return inputX; }
	float GetCurSpeed() const { return curSpeed; }

	bool GetIsWallLeft() const { return isWallLeft; }
	bool GetIsWallRight() const { return isWallRight; }

	bool GetLastFlipX() const { return lastFlipX; } // true ���� 
	void SetLastFlipX(bool isFlipX) { lastFlipX = isFlipX; } // true ���� 
	bool GetisBulletFliping() const { return isBulletFliping; }
	void SetisBulletFliping(bool isBulletTime) { isBulletFliping = isBulletTime; }
	bool GetisBulletFlipX() const { return isBulletFlipX; } // true ���� 
	void SetisBulletFlipX(bool isBulletX) { isBulletFlipX = isBulletX; }

	Rigidbody* GetRigidbody() const { return rigidbody; }
	AnimatorController* GetAnimatorController() const { return animatorController; }
    Transform* GetTransform() const { return transform; }
	// BoxCollider* GetBoxCollider() const { return boxCollider; }

	// void SetLastWallDir(int dir) { lastWallDir = dir; }


public:
	// script component cycle
	void OnEnable() override;				// ������Ʈ Ȱ��ȭ ����
	void Awake() override;					// ������Ʈ�� ��� ������Ʈ�� ������ ���� ���� 1ȸ ȣ��
	void Start() override;					// Awake() ���� ���� 1ȸ ȣ��
	void Update() override;					// �� ������ �ݺ� ȣ��
	void FixedUpdate() override;			// ���� ������Ʈ 0.02f ���� �ݺ� ȣ��
	void OnDestroy() override;				// ������Ʈ or ������Ʈ �Ҹ� ����

	// trigger
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override {}
	void OnTriggerStay(ICollider* other, const ContactInfo& contact)  override {}
	void OnTriggerExit(ICollider* other, const ContactInfo& contact)  override {}

	// collision
	void OnCollisionEnter(ICollider* other, const ContactInfo& contact)  override
	{
		if (other->gameObject->name == "Ground" && contact.normal.y > 0.5)
		{ 
			OutputDebugStringA("Ground�� �浹 �߽��ϴ�.\n");
			isGround = true; 
		}
		else if (other->gameObject->name == "Wall")
		{
			OutputDebugStringA("Wall�� �浹 �߽��ϴ�.\n");
			if (contact.normal.x == 1) { isWallLeft = true; }
			if (contact.normal.x == -1) { isWallRight = true; }
		}
	}

	void OnCollisionStay(ICollider* other, const ContactInfo& contact) override {}

	void OnCollisionExit(ICollider* other, const ContactInfo& contact)  override
	{
		if (other->gameObject->name == "Ground" && contact.normal.y > 0.5)
		{ 
			OutputDebugStringA("Ground ���������� .\n");
			isGround = false; 
		}
		else if (other->gameObject->name == "Wall")
		{
			if (contact.normal.x == 1)   isWallLeft = false;
			if (contact.normal.x == -1)  isWallRight = false;
		}
	}

public:
	float GetSpeed() { return curSpeed; }

private:
	void InputCheak();
};

