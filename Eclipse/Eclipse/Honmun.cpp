#include "Honmun.h"
#include "HonmunCollisionScript.h"
#include "HonmunAFSM.h"
#include "HonmunAAnimatorController.h"
#include "HonmunBFSM.h"
#include "HonmunBAnimatorController.h"
#include "HonmunCFSM.h"
#include "HonmunCAnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include <iostream>
#include <exception>

Honmun::Honmun() : GameObject("Honmun"), honmunType(HonmunType::A), honmunAFSM(nullptr), honmunAAnimatorController(nullptr), honmunBFSM(nullptr), honmunBAnimatorController(nullptr), honmunCFSM(nullptr), honmunCAnimatorController(nullptr)
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
	rigidbody = AddComponent<Rigidbody>();
	collider = AddComponent<CircleCollider>();
	animator = AddComponent<Animator>();

	// �浹 ��ũ��Ʈ �߰�
	auto* collisionScript = AddComponent<HonmunCollisionScript>();
}

void Honmun::Awake()
{
	// 임시로 모든 타입에 정적 스프라이트 사용 (애니메이션 시스템 비활성화)
	auto texture = ResourceManager::Get().CreateTexture2D(GetTexturePath());
	spriteRenderer->sprite = ResourceManager::Get().CreateSprite(texture, GetSpriteName());
	
	// TODO: 나중에 A 타입에 애니메이션 시스템 다시 적용
	/*
	if (honmunType == HonmunType::A)
	{
		// 애니메이션 시스템 - 안정화 후 다시 적용 예정
	}
	*/

	// \ubb3c\ub9ac \uc124\uc815 - \uc6e8\uc774\ube0c 1 \ud14c\uc2a4\ud2b8\uc6a9 \ud0a4\ub124\ub9c8\ud2f1 \ubaa8\ub4dc
	rigidbody->useGravity = false;  // \uc911\ub825 \ube44\ud65c\uc131\ud654
	rigidbody->isKinematic = true;  // \ud0a4\ub124\ub9c8\ud2f1 \ubaa8\ub4dc\ub85c \uc124\uc815

	// 스프라이트 크기를 원본 크기로 설정
	transform->SetScale(1.0f, 1.0f);
	
	// 타입별 콜라이더 설정
	SetupColliderForType();
}

void Honmun::SceneStart()
{
	// �� ���� �� �浹 ��ũ��Ʈ�� Ÿ�� ����
	auto* collisionScript = GetComponent<HonmunCollisionScript>();
	if (collisionScript)
	{
		collisionScript->SetHonmunType(honmunType);
	}
}

void Honmun::Update()
{
	// ����� �ݶ��̴� �׸���
	if (collider)
	{
		collider->DebugColliderDraw();
	}
}

Honmun::~Honmun()
{
	// Clean up dynamically allocated objects
	if (honmunAAnimatorController)
	{
		delete honmunAAnimatorController;
		honmunAAnimatorController = nullptr;
	}
	if (honmunBAnimatorController)
	{
		delete honmunBAnimatorController;
		honmunBAnimatorController = nullptr;
	}
	if (honmunCAnimatorController)
	{
		delete honmunCAnimatorController;
		honmunCAnimatorController = nullptr;
	}
}

void Honmun::Destroyed()
{
	// ������Ʈ �ı� �� ���� �۾�
}

void Honmun::SetHonmunType(HonmunType type)
{
	honmunType = type;

	// Clean up existing FSM components when switching types
	if (honmunAAnimatorController && type != HonmunType::A)
	{
		delete honmunAAnimatorController;
		honmunAAnimatorController = nullptr;
	}
	if (honmunBAnimatorController && type != HonmunType::B)
	{
		delete honmunBAnimatorController;
		honmunBAnimatorController = nullptr;
	}
	if (honmunCAnimatorController && type != HonmunType::C)
	{
		delete honmunCAnimatorController;
		honmunCAnimatorController = nullptr;
	}

	// Initialize FSM and Animator based on type
	if (type == HonmunType::A)
	{
		// Create and setup animator controller for Honmun A
		if (!honmunAAnimatorController)
		{
			honmunAAnimatorController = new HonmunAAnimatorController();
		}
		if (animator)
		{
			animator->SetController(honmunAAnimatorController);
		}
		
		// Add FSM component for Honmun A if not already present
		if (!GetComponent<HonmunAFSM>())
		{
			honmunAFSM = AddComponent<HonmunAFSM>();
		}
	}
	else if (type == HonmunType::B)
	{
		// Create and setup animator controller for Honmun B
		if (!honmunBAnimatorController)
		{
			honmunBAnimatorController = new HonmunBAnimatorController();
		}
		if (animator)
		{
			animator->SetController(honmunBAnimatorController);
		}
		
		// Add FSM component for Honmun B if not already present
		if (!GetComponent<HonmunBFSM>())
		{
			honmunBFSM = AddComponent<HonmunBFSM>();
		}
	}
	else if (type == HonmunType::C)
	{
		// 임시로 C 타입도 정적 스프라이트만 사용 (애니메이션 시스템 안정화될 때까지)
		if (spriteRenderer)
		{
			auto texture = ResourceManager::Get().CreateTexture2D(GetTexturePath());
			spriteRenderer->sprite = ResourceManager::Get().CreateSprite(texture, GetSpriteName());
		}
		
		// TODO: 애니메이션 시스템 안정화 후 다시 활성화
		/*
		// Create and setup animator controller for Honmun C
		if (!honmunCAnimatorController)
		{
			honmunCAnimatorController = new HonmunCAnimatorController();
		}
		if (animator)
		{
			animator->SetController(honmunCAnimatorController);
		}
		
		// Add FSM component for Honmun C if not already present
		if (!GetComponent<HonmunCFSM>())
		{
			honmunCFSM = AddComponent<HonmunCFSM>();
		}
		*/
	}
	else
	{
		// For other types (D), use static sprites
		if (spriteRenderer)
		{
			auto texture = ResourceManager::Get().CreateTexture2D(GetTexturePath());
			spriteRenderer->sprite = ResourceManager::Get().CreateSprite(texture, GetSpriteName());
		}
	}

	// 타입 변경 시 콜라이더 재설정
	SetupColliderForType();
	
	// �浹 ��ũ��Ʈ���� Ÿ�� ����
	auto* collisionScript = GetComponent<HonmunCollisionScript>();
	if (collisionScript)
	{
		collisionScript->SetHonmunType(type);
	}
}

void Honmun::SetPosition(float x, float y)
{
	if (transform)
	{
		transform->SetPosition(x, y);
	}
}

void Honmun::SetAlpha(float alpha)
{
	if (spriteRenderer)
	{
		spriteRenderer->alpha = alpha;
	}
}

void Honmun::ResetAlpha()
{
	if (spriteRenderer)
	{
		spriteRenderer->alpha = 1.0f;
	}
}

std::string Honmun::GetTexturePath()
{
	switch (honmunType)
	{
	case HonmunType::A: return "../Resource/Aron/Honmun_a.png";
	case HonmunType::B: return "../Resource/Aron/Honmun_b.png";
	case HonmunType::C: return "../Resource/Aron/Honmun_c.png";
	case HonmunType::D: return "../Resource/Aron/Honmun_d.png";
	default: return "../Resource/Aron/Honmun_a.png";
	}
}

std::string Honmun::GetSpriteName()
{
	switch (honmunType)
	{
	case HonmunType::A: return "Honmun_A";
	case HonmunType::B: return "Honmun_B";
	case HonmunType::C: return "Honmun_C";
	case HonmunType::D: return "Honmun_D";
	default: return "Honmun_A";
	}
}

void Honmun::SetSize(float newSize)
{
	size = newSize;
	if (transform)
	{
		transform->SetScale(size, size); // 원본 크기에 새로운 크기 적용
	}
	SetupColliderForType(); // 크기 변경 시 콜라이더도 재설정
}

void Honmun::SetupColliderForType()
{
	if (!collider) return;
	
	collider->isTrigger = true; // 트리거 모드로 변경
	
	switch (honmunType)
	{
	case HonmunType::A:
		collider->radius = 35.0f * size;
		collider->offset.y = -11.0f; // A타입은 살짝 아래로
		break;
	case HonmunType::B:
		collider->radius = 34.0f * size;
		collider->offset.y = -23.0f; // B타입은 더 아래로
		break;
	case HonmunType::C:
		collider->radius = 50.0f * size;
		collider->offset.y = -10.0f; // C타입은 A와 비슷
		break;
	case HonmunType::D:
		collider->radius = 50.0f * size;
		collider->offset.y = -10.0f; // D타입은 A와 비슷
		break;
	default:
		collider->radius = 50.0f * size;
		collider->offset.y = -10.0f;
		break;
	}
}

void Honmun::AdjustSpritePosition()
{
	if (spriteRenderer && spriteRenderer->sprite)
	{
		// 아래쪽이 잘리지 않도록 살짝 위로 이동
		float offsetY = 15.0f; // 위로 15픽셀 이동 (미세 조정)
		
		auto currentPos = transform->GetPosition();
		transform->SetPosition(currentPos.x, currentPos.y + offsetY);
	}
}