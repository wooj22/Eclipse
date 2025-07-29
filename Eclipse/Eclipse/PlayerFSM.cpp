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

// 컴포넌트 활성화 시점
void PlayerFSM::OnEnable()
{

}

void PlayerFSM::Awake()
{
	transform = gameObject->GetComponent<Transform>();
	spriteRenderer = gameObject->GetComponent<SpriteRenderer>();
	rigidbody = gameObject->GetComponent<Rigidbody>();


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

	// [ Speed Setting ]
	if (isA || isD)
	{
		if (!isShift) curSpeed = walkSpeed;
		else curSpeed = runSpeed;
	}
	else curSpeed = 0;


	// [ FlipX Setting ]
	if (isA || isD)
	{
		spriteRenderer->flipX = Input::GetAxisHorizontal() >= 0 ? false : true;
		lastFlipX = spriteRenderer->flipX;
	}
	else spriteRenderer->flipX = lastFlipX;

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