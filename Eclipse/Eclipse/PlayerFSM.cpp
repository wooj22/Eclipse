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

	actionFSM = std::make_unique<ActionFSM>();
	actionFSM->Init(this);
}

void PlayerFSM::Start()
{

}

void PlayerFSM::Update()
{
	InputCheak(); // input Ű�� Ȯ��

	movementFSM->Update();
	actionFSM->Update();

	MouseWorldPos = Camera::GetScreenToWorldPosition(Input::GetMouseScreenPosition());

	// [ Speed Setting ]
	if (isA || isD)
	{
		if (!isShift) curSpeed = walkSpeed;
		else curSpeed = dashSpeed;
	}
	else curSpeed = 0;


	// [ FlipX Setting - ���� �̵� ���� ���� ]
	if (abs(rigidbody->velocity.x) > 0.01f)  // ���� ���°� �ƴ� ���� ���� �ݿ�
	{
		spriteRenderer->flipX = rigidbody->velocity.x < 0.0f;  // �������� �̵� ���̸� flip
		lastFlipX = spriteRenderer->flipX;
	}
	else
	{
		spriteRenderer->flipX = lastFlipX;  // �ӵ��� ���� 0�̸� ���� ���� ����
	}

	// [ animation ]
	// animatorController->SetFloat("Dash", dashSpeed);
}

void PlayerFSM::FixedUpdate()
{
	movementFSM->FixedUpdate();
}

void PlayerFSM::OnDestroy()
{

}

void PlayerFSM::InputCheak()
{
	inputX = Input::GetAxisHorizontal();
	inputY = Input::GetAxisVertical();

	// isW = Input::GetKey('W');
	isA = Input::GetKey('A');
	isD = Input::GetKey('D');
	isShift = Input::GetKey(VK_SHIFT);
	isSpace = Input::GetKeyDown(VK_SPACE);

	isLButton = Input::GetKeyDown(VK_LBUTTON);
	isRButton = Input::GetKeyDown(VK_RBUTTON);
}