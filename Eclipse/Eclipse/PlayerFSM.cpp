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

	actionFSM = std::make_unique<ActionFSM>();
	actionFSM->Init(this);
}

void PlayerFSM::Start()
{

}

void PlayerFSM::Update()
{
	InputCheak(); // input 키값 확인

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


	// [ FlipX Setting - 실제 이동 방향 기준 ]
	if (abs(rigidbody->velocity.x) > 0.01f)  // 정지 상태가 아닐 때만 방향 반영
	{
		spriteRenderer->flipX = rigidbody->velocity.x < 0.0f;  // 왼쪽으로 이동 중이면 flip
		lastFlipX = spriteRenderer->flipX;
	}
	else
	{
		spriteRenderer->flipX = lastFlipX;  // 속도가 거의 0이면 이전 방향 유지
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