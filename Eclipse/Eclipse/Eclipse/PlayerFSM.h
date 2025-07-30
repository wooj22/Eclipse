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

class PlayerFSM : public Script
{
private:
	std::unique_ptr<MovementFSM> movementFSM;
	std::unique_ptr<ActionFSM> actionFSM;

public:
	MovementFSM* GetMovementFSM() { return movementFSM.get(); }
	ActionFSM* GetActionFSM() { return actionFSM.get(); }

private:
	// stat
	float curSpeed = 0;
	float walkSpeed = 280.f;
	float runSpeed = 350.f;
	float jumpForce = 350.0f;

	// move
	float inputX, inputY;
	bool isGround;
	bool lastFlipX = false;

	bool isWallLeft = false;
	bool isWallRight = false;

	// key
	bool isA, isD, isShift, isSpace, isLButton, isRButton;

	// ref component
	Transform* transform = nullptr;
	SpriteRenderer* spriteRenderer = nullptr;
	Rigidbody* rigidbody = nullptr;
	AnimatorController* animatorController = nullptr;

public:
	// getter
	bool GetIsGround() const { return isGround; }
	bool GetIsSpace() const { return isSpace; }
	bool GetisShift() const { return isShift; }   // �ӽ� �뽬
	bool GetIsA() const { return isA; }
	bool GetIsD() const { return isD; }
	bool GetIsLButton() const { return isLButton; }
	bool GetIsRButton() const { return isRButton; }

	float GetJumpForce() const { return jumpForce; }
	float GetInputX() const { return inputX; }
	float GetCurSpeed() const { return curSpeed; }

	bool GetIsWallLeft() const { return isWallLeft; }
	bool GetIsWallRight() const { return isWallRight; }

	Rigidbody* GetRigidbody() const { return rigidbody; }

public:
	// script component cycle
	void OnEnable() override;				// ������Ʈ Ȱ��ȭ ����
	void Awake() override;					// ������Ʈ�� ��� ������Ʈ�� ������ ���� ���� 1ȸ ȣ��
	void Start() override;					// Awake() ���� ���� 1ȸ ȣ��
	void Update() override;					// �� ������ �ݺ� ȣ��
	void FixedUpdate() override;			// ���� ������Ʈ 0.02f ���� �ݺ� ȣ��
	void OnDestroy() override;				// ������Ʈ or ������Ʈ �Ҹ� ����

	// trigger
	void OnTriggerEnter(ICollider* other) override {}
	void OnTriggerStay(ICollider* other)  override {}
	void OnTriggerExit(ICollider* other)  override {}

	// collision
	void OnCollisionEnter(ICollider* other, const ContactInfo& contact)  override
	{
		if (other->gameObject->name == "Ground") { isGround = true; }
		else if (other->gameObject->name == "Wall")
		{
			if (contact.normal.x == 1) { isWallLeft = true; }
			if (contact.normal.x == -1) { isWallRight = true; }
		}
	}

	void OnCollisionStay(ICollider* other, const ContactInfo& contact) override {}

	void OnCollisionExit(ICollider* other, const ContactInfo& contact)  override
	{
		if (other->gameObject->name == "Ground") { isGround = false; }
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
	//void Move_Transform();		// kinematic move
	//void Move_Physics();		    // physics move
	//void Jump_Physics();
};

