#include "HonmunCollisionScript.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/Time.h"
#include <random>
#include <algorithm>

void HonmunCollisionScript::Awake()
{
	// Honmun�� GameObject�̹Ƿ� ���� ĳ����
	honmun = dynamic_cast<Honmun*>(gameObject);

	// �������� ������Ʈ�� ��������
	transform = gameObject->GetComponent<Transform>();
	rigidbody = gameObject->GetComponent<Rigidbody>();
	spriteRenderer = gameObject->GetComponent<SpriteRenderer>();
}

void HonmunCollisionScript::Start()
{
	// �ʱ� ����
	started = true;
}

void HonmunCollisionScript::Update()
{
	// ��ٿ� ó��
	if (reactionCooldown > 0)
	{
		reactionCooldown -= Time::GetDeltaTime();
	}

	// ���� �ӵ� ���� (�߷� ���� �߰� �ӵ�)
	if (rigidbody && !rigidbody->isKinematic)
	{
		rigidbody->velocity.y -= fallingSpeed * 0.1f;
	}
}

void HonmunCollisionScript::OnCollisionEnter(ICollider* other, const ContactInfo& contact)
{
	// ��ٿ� ���̸� ����
	if (reactionCooldown > 0 || isProcessingReaction) return;

	HonmunCollisionScript* otherScript = GetHonmunScript(other);
	if (!otherScript) return;

	// ������� �÷��� ����
	isProcessingReaction = true;
	otherScript->isProcessingReaction = true;

	// Ÿ�Ժ� ���� ó��
	if (honmunType == otherScript->honmunType)
	{
		// ���� Ÿ�Գ��� �浹
		switch (honmunType)
		{
		case HonmunType::A: // Ignis - ��ü
			HandleIgnisReaction(otherScript, contact);
			break;
		case HonmunType::B: // Umbra - �п�
			HandleUmbraReaction(otherScript, contact);
			break;
		case HonmunType::C: // Darkness - ����
			HandleDarknessReaction(otherScript, contact);
			break;
		case HonmunType::D: // Luna - ����
			HandleLunaReaction(otherScript, contact);
			break;
		}
	}
	else
	{
		// �ٸ� Ÿ�Գ��� �浹
		HandleMixedReaction(otherScript, contact);
	}

	// ��ٿ� ����
	reactionCooldown = 0.5f;
	isProcessingReaction = false;
	if (otherScript) otherScript->isProcessingReaction = false;
}

void HonmunCollisionScript::SetHonmunType(HonmunType type)
{
	honmunType = type;

	// Ÿ�Ժ� �⺻ �Ӽ� ����
	switch (type)
	{
	case HonmunType::A: // Ignis
		health = 3;
		currentSize = 10.0f;
		fallingSpeed = 1.0f;
		break;
	case HonmunType::B: // Umbra
		health = 3;
		currentSize = 10.0f;
		fallingSpeed = 1.0f;
		break;
	case HonmunType::C: // Darkness
		health = 1;
		currentSize = 10.0f;
		fallingSpeed = 2.0f;
		break;
	case HonmunType::D: // Luna
		health = 1;
		currentSize = 5.0f;
		fallingSpeed = 1.0f;
		break;
	}
}

void HonmunCollisionScript::HandleIgnisReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact)
{
	// A + A = ��ü (ũ�� 10% ����, ���ϼӵ� 20% ����)
	MergeWithOther(otherScript);
}

void HonmunCollisionScript::HandleUmbraReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact)
{
	// B + B = �п� (���� 2���� �и�, ũ�� 30% ����, ���ϼӵ� 20% ����)
	SplitIntoTwo();
	otherScript->SplitIntoTwo();
}

void HonmunCollisionScript::HandleDarknessReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact)
{
	// C + C = ���� (ī�޶� ���� �� ������ �浹�������� ������� �ı�)
	Vector2 collisionPoint = contact.point;
	AbsorbNearbyEnemies(collisionPoint);

	// �ڽŵ鵵 �ı�
	DestroyThis();
	otherScript->DestroyThis();
}

void HonmunCollisionScript::HandleLunaReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact)
{
	// D + any = ���� (��� �浹���� �ı�)
	DestroyThis();
	if (otherScript->honmunType == HonmunType::D)
	{
		otherScript->DestroyThis();
	}
}

void HonmunCollisionScript::HandleMixedReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact)
{
	// D�� ���Ե� �浹�� �׻� D�� �ı���
	if (honmunType == HonmunType::D)
	{
		DestroyThis();
		return;
	}
	if (otherScript->honmunType == HonmunType::D)
	{
		otherScript->DestroyThis();
		return;
	}

	// A&B �Ǵ� B&A - ƨ��
	if ((honmunType == HonmunType::A && otherScript->honmunType == HonmunType::B) ||
		(honmunType == HonmunType::B && otherScript->honmunType == HonmunType::A))
	{
		BounceAway(otherScript, contact);
	}
	// A&C �Ǵ� C&A - �и�
	else if ((honmunType == HonmunType::A && otherScript->honmunType == HonmunType::C) ||
		(honmunType == HonmunType::C && otherScript->honmunType == HonmunType::A))
	{
		PushSideways(otherScript);
	}
	// B&C �Ǵ� C&B - ����
	else if ((honmunType == HonmunType::B && otherScript->honmunType == HonmunType::C) ||
		(honmunType == HonmunType::C && otherScript->honmunType == HonmunType::B))
	{
		PassThrough(otherScript);
	}
}

void HonmunCollisionScript::MergeWithOther(HonmunCollisionScript* otherScript)
{
	// ũ�� 10% ����
	UpdateSize(currentSize * 1.1f);

	// ���ϼӵ� 20% ����
	UpdateFallingSpeed(0.8f);

	// ���� ����
	otherScript->DestroyThis();
}

void HonmunCollisionScript::SplitIntoTwo()
{
	// ũ�� 30% ����
	UpdateSize(currentSize * 0.7f);

	// ���ϼӵ� 20% ����
	UpdateFallingSpeed(1.2f);

	// ���ο� ȥ�� ����
	Vector2 currentPos = transform->GetPosition();
	Vector2 offset = GetRandomDirection() * 2.0f;

	auto* newHonmun = Instantiate<Honmun>(currentPos + offset);
	auto* newScript = newHonmun->AddComponent<HonmunCollisionScript>();
	newScript->SetHonmunType(honmunType);
	newScript->UpdateSize(currentSize);
	newScript->UpdateFallingSpeed(fallingSpeed);
}

void HonmunCollisionScript::AbsorbNearbyEnemies(const Vector2& collisionPoint)
{
	auto nearbyHonmuns = GetNearbyHonmuns(100.0f); // 100 �ȼ� �ݰ�

	for (auto* honmunScript : nearbyHonmuns)
	{
		if (honmunScript == this) continue;

		// ī�޶� ���� ���� �ִ��� Ȯ��
		if (honmunScript->IsInCameraView())
		{
			// �浹�������� �������
			Vector2 direction = collisionPoint - honmunScript->transform->GetPosition();
			direction = direction.Normalized();

			// �и��� �Ÿ��� 1/3��ŭ �̵�
			Vector2 pullForce = direction * (pushDistance / 3.0f);
			honmunScript->rigidbody->AddImpulse(pullForce);

			// ���� �ð� �� �ı� (���� ȿ��)
			honmunScript->reactionCooldown = 1.0f; // 1�� �� �ı��ǵ��� ����
		}
	}
}

void HonmunCollisionScript::DestroyThis()
{
	if (gameObject)
	{
		gameObject->SetActive(false); // �Ǵ� Destroy �Լ� ���
	}
}

void HonmunCollisionScript::BounceAway(HonmunCollisionScript* otherScript, const ContactInfo& contact)
{
	// �������� �ݴ�� ƨ�ܳ�
	Vector2 bounceDirection = -contact.normal;
	Vector2 bounceForce = bounceDirection * pushDistance;

	rigidbody->AddImpulse(bounceForce);
	otherScript->rigidbody->AddImpulse(-bounceForce);
}

void HonmunCollisionScript::PushSideways(HonmunCollisionScript* otherScript)
{
	// �¿�� �о
	Vector2 leftDirection(-1.0f, 0.0f);
	Vector2 rightDirection(1.0f, 0.0f);

	rigidbody->AddImpulse(leftDirection * pushDistance);
	otherScript->rigidbody->AddImpulse(rightDirection * pushDistance);
}

void HonmunCollisionScript::PassThrough(HonmunCollisionScript* otherScript)
{
	// ���� - ������ �浹�� ���������� ��ġ���� �ʰ� ��
	// �Ͻ������� �ݶ��̴��� Ʈ���ŷ� �����ϰų� ��ġ�� �ణ ����
	Vector2 separationDirection = (transform->GetPosition() - otherScript->transform->GetPosition()).Normalized();
	Vector2 separationForce = separationDirection * 1.0f;

	rigidbody->AddImpulse(separationForce);
	otherScript->rigidbody->AddImpulse(-separationForce);
}

std::vector<HonmunCollisionScript*> HonmunCollisionScript::GetNearbyHonmuns(float radius)
{
	std::vector<HonmunCollisionScript*> nearbyHonmuns;

	// ���� ���� ��� ���ӿ�����Ʈ���� HonmunCollisionScript�� ���� �͵��� ã��
	// �� �κ��� �� ���� �ý��ۿ� ���� ���� ����� �޶��� �� �ֽ��ϴ�

	return nearbyHonmuns;
}

Vector2 HonmunCollisionScript::GetRandomDirection()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

	return Vector2(dis(gen), dis(gen)).Normalized();
}

void HonmunCollisionScript::UpdateSize(float newSize)
{
	currentSize = newSize;
	if (transform)
	{
		float scaleFactor = newSize / 10.0f; // �⺻ ũ�⸦ 10���� ����
		transform->SetScale(scaleFactor, scaleFactor);
	}
}

void HonmunCollisionScript::UpdateFallingSpeed(float speedMultiplier)
{
	fallingSpeed *= speedMultiplier;
}

bool HonmunCollisionScript::IsInCameraView()
{
	// ī�޶� ���� ���� �ִ��� Ȯ���ϴ� ����
	// ���� ������ ī�޶� �ý��ۿ� ���� �޶��� �� �ֽ��ϴ�
	return true; // �ӽ�
}

HonmunCollisionScript* HonmunCollisionScript::GetHonmunScript(ICollider* collider)
{
	if (!collider || !collider->gameObject) return nullptr;

	return collider->gameObject->GetComponent<HonmunCollisionScript>();
}