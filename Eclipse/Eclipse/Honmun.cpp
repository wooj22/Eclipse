#include "Honmun.h"

Honmun::Honmun() : GameObject("Honmun"), honmunType(HonmunType::A)
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
	rigidbody = AddComponent<Rigidbody>();
	collider = AddComponent<CircleCollider>();
}

void Honmun::Awake()
{
	// �ؽ�ó �ε� �� ��������Ʈ ����
	auto texture = ResourceManager::Get().CreateTexture2D(GetTexturePath());
	spriteRenderer->sprite = ResourceManager::Get().CreateSprite(texture, GetSpriteName());

	// ���� ����
	rigidbody->useGravity = true;
	rigidbody->isKinematic = false;

	// �ݶ��̴� ����
	collider->radius = 50.0f;

	// ������ ����
	transform->SetScale(1.0f, 1.0f);
}

void Honmun::SceneStart()
{
	// �� ���� �� �ʿ��� �ʱ�ȭ
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