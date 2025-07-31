#include "Honmun.h"
#include "HonmunCollisionScript.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

Honmun::Honmun() : GameObject("Honmun"), honmunType(HonmunType::A)
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
	rigidbody = AddComponent<Rigidbody>();
	collider = AddComponent<CircleCollider>();

	// �浹 ��ũ��Ʈ �߰�
	auto* collisionScript = AddComponent<HonmunCollisionScript>();
}

void Honmun::Awake()
{
	// �ؽ�ó �ε� �� ��������Ʈ ����
	auto texture = ResourceManager::Get().CreateTexture2D(GetTexturePath());
	spriteRenderer->sprite = ResourceManager::Get().CreateSprite(texture, GetSpriteName());

	// \ubb3c\ub9ac \uc124\uc815 - \uc6e8\uc774\ube0c 1 \ud14c\uc2a4\ud2b8\uc6a9 \ud0a4\ub124\ub9c8\ud2f1 \ubaa8\ub4dc
	rigidbody->useGravity = false;  // \uc911\ub825 \ube44\ud65c\uc131\ud654
	rigidbody->isKinematic = true;  // \ud0a4\ub124\ub9c8\ud2f1 \ubaa8\ub4dc\ub85c \uc124\uc815

	// �ݶ��̴� ����
	collider->radius = 50.0f;
	collider->isTrigger = false; // ������ �浹�� ���� false

	// ������ ����
	transform->SetScale(1.0f, 1.0f);
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

void Honmun::Destroyed()
{
	// ������Ʈ �ı� �� ���� �۾�
}

void Honmun::SetHonmunType(HonmunType type)
{
	honmunType = type;

	// Ÿ���� ������ �� ���ҽ��� �ٽ� �ε�
	if (spriteRenderer)
	{
		auto texture = ResourceManager::Get().CreateTexture2D(GetTexturePath());
		spriteRenderer->sprite = ResourceManager::Get().CreateSprite(texture, GetSpriteName());
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