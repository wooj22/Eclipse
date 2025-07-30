#include "Honmun.h"
#include "HonmunCollisionScript.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

Honmun::Honmun() : GameObject("Honmun"), honmunType(HonmunType::A)
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
	rigidbody = AddComponent<Rigidbody>();
	collider = AddComponent<CircleCollider>();

	// 충돌 스크립트 추가
	auto* collisionScript = AddComponent<HonmunCollisionScript>();
}

void Honmun::Awake()
{
	// 텍스처 로드 및 스프라이트 생성
	auto texture = ResourceManager::Get().CreateTexture2D(GetTexturePath());
	spriteRenderer->sprite = ResourceManager::Get().CreateSprite(texture, GetSpriteName());

	// 물리 설정
	rigidbody->useGravity = true;
	rigidbody->isKinematic = false;

	// 콜라이더 설정
	collider->radius = 50.0f;
	collider->isTrigger = false; // 물리적 충돌을 위해 false

	// 스케일 설정
	transform->SetScale(1.0f, 1.0f);
}

void Honmun::SceneStart()
{
	// 씬 시작 시 충돌 스크립트에 타입 설정
	auto* collisionScript = GetComponent<HonmunCollisionScript>();
	if (collisionScript)
	{
		collisionScript->SetHonmunType(honmunType);
	}
}

void Honmun::Update()
{
	// 디버그 콜라이더 그리기
	if (collider)
	{
		collider->DebugColliderDraw();
	}
}

void Honmun::Destroyed()
{
	// 오브젝트 파괴 시 정리 작업
}

void Honmun::SetHonmunType(HonmunType type)
{
	honmunType = type;

	// 타입이 설정된 후 리소스를 다시 로드
	if (spriteRenderer)
	{
		auto texture = ResourceManager::Get().CreateTexture2D(GetTexturePath());
		spriteRenderer->sprite = ResourceManager::Get().CreateSprite(texture, GetSpriteName());
	}

	// 충돌 스크립트에도 타입 설정
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