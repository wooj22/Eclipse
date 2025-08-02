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

	// actionFSM = std::make_unique<ActionFSM>();
	// actionFSM->Init(this);
}

void PlayerFSM::Start()
{
	// [ 스킬 해금 ] 테스트 위해서 
	// for (int i = 0; i < 3; ++i) 
	GameManager::Get().LevelUpSkill(SkillType::KnockbackDistanceUp); 
	GameManager::Get().LevelUpSkill(SkillType::KnockbackDistanceUp);
	GameManager::Get().LevelUpSkill(SkillType::KnockbackDistanceUp);
	GameManager::Get().LevelUpSkill(SkillType::DoubleJump);
	GameManager::Get().LevelUpSkill(SkillType::WallJump);

	// GameManager::Get().LevelUpSkill(SkillType::SkillCooldownDown);
	// GameManager::Get().LevelUpSkill(SkillType::SkillCooldownDown);
	// GameManager::Get().LevelUpSkill(SkillType::JumpAttackExtra);
	// GameManager::Get().LevelUpSkill(SkillType::FastFall);

	// GameManager::Get().LevelUpSkill(SkillType::MoveSpeedUp);
	// GameManager::Get().LevelUpSkill(SkillType::MoveSpeedUp);
	// GameManager::Get().LevelUpSkill(SkillType::MoveSpeedUp);
	// GameManager::Get().LevelUpSkill(SkillType::AttackRangeUp);
	// GameManager::Get().LevelUpSkill(SkillType::AttackRangeUp);
	// GameManager::Get().LevelUpSkill(SkillType::AttackRangeUp);
	// GameManager::Get().LevelUpSkill(SkillType::Dash);
}

void PlayerFSM::Update()
{
	InputCheak(); // input 키값 확인

	movementFSM->Update();
	// actionFSM->Update();

	MouseWorldPos = Camera::GetScreenToWorldPosition(Input::GetMouseScreenPosition());

	// [ Speed Setting ]
	if (isA || isD)
	{
		if (!isShift) curSpeed = walkSpeed * speedDownRate;
		else curSpeed = dashSpeed * speedDownRate;
	}
	else curSpeed = 0;


	// [ FlipX Setting - 실제 이동 방향 기준 ]
	if (!isBulletFliping)
	{
		if (abs(rigidbody->velocity.x) > 0.01f)   // 정지 상태가 아닐 때만 방향 반영
		{
			spriteRenderer->flipX = rigidbody->velocity.x < 0.0f;  // 왼쪽으로 이동 중이면 flip
			lastFlipX = spriteRenderer->flipX;
		}
		else
		{
			spriteRenderer->flipX = lastFlipX;  // 속도가 거의 0이면 이전 방향 유지
		}
	}
	else
	{
		spriteRenderer->flipX = isBulletFlipX;  // BulletTime_State 에서 변수값 조정
	}


	// [ FSM 상태 ] 
	//MovementStateBase* currentState = playerFSM->GetMovementFSM()->GetCurrentState();
	//if (currentState)
	//{
	//	std::string name = typeid(*currentState).name();  // 상태 이름 확인
	//	OutputDebugStringA(("현재 상태: " + name + "\n").c_str());
	//}


	// [ animation ]
	// animatorController->SetFloat("Dash", dashSpeed);


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

void PlayerFSM::InputCheak()
{
	inputX = Input::GetAxisHorizontal();
	inputY = Input::GetAxisVertical();

	// isW = Input::GetKey('W');
	isA = Input::GetKey('A');
	isD = Input::GetKey('D');
	isS = Input::GetKey('S');
	isShift = Input::GetKey(VK_SHIFT);
	isSpace = Input::GetKeyDown(VK_SPACE);

	isLButton = Input::GetKeyDown(VK_LBUTTON);
	isRButton = Input::GetKeyDown(VK_RBUTTON);

	isF = Input::GetKey('F');
}



// *-------------- [ Collider ] --------------*

// trigger

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


// collision

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