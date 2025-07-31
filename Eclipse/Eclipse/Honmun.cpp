#include "Honmun.h"
#include "HonmunCollisionScript.h"
#include "HonmunAFSM.h"
#include "HonmunAAnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include <iostream>
#include <exception>

Honmun::Honmun() : GameObject("Honmun"), honmunType(HonmunType::A), honmunAFSM(nullptr), honmunAAnimatorController(nullptr)
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

	// 콜라이더 설정 (스프라이트 크기에 맞게 조정)
	collider->radius = 35.0f; // 70% 크기에 맞게 조정 (50 * 0.7 = 35)
	collider->isTrigger = false;
	
	// 콜라이더를 스프라이트보다 살짝 아래로 이동
	collider->offset.y = -10.0f; // 아래로 10픽셀 이동

	// 스프라이트 크기 조정 (아래쪽 잘림 방지)
	transform->SetScale(0.7f, 0.7f); // 크기를 70%로 줄임
	
	// 아래쪽만 살짝 조정하기 위해 위치 미세 조정
	AdjustSpritePosition();
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
}

void Honmun::Destroyed()
{
	// ������Ʈ �ı� �� ���� �۾�
}

void Honmun::SetHonmunType(HonmunType type)
{
	honmunType = type;

	// Initialize FSM and Animator for Honmun A
	if (type == HonmunType::A)
	{
		// Clean up existing animator controller if switching types
		if (honmunAAnimatorController)
		{
			delete honmunAAnimatorController;
		}
		
		// Create and setup animator controller for Honmun A
		honmunAAnimatorController = new HonmunAAnimatorController();
		if (animator)
		{
			animator->SetController(honmunAAnimatorController);
		}
		
		// Add FSM component for Honmun A if not already present
		if (!honmunAFSM)
		{
			honmunAFSM = AddComponent<HonmunAFSM>();
		}
	}
	else
	{
		// For other types (B, C, D), use static sprites and clean up A-specific components
		if (honmunAAnimatorController)
		{
			delete honmunAAnimatorController;
			honmunAAnimatorController = nullptr;
		}
		
		if (spriteRenderer)
		{
			auto texture = ResourceManager::Get().CreateTexture2D(GetTexturePath());
			spriteRenderer->sprite = ResourceManager::Get().CreateSprite(texture, GetSpriteName());
		}
	}

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