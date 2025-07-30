#include "HonmunCollisionScript.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/Time.h"
#include <random>
#include <algorithm>

void HonmunCollisionScript::Awake()
{
	// Honmun은 GameObject이므로 직접 캐스팅
	honmun = dynamic_cast<Honmun*>(gameObject);

	// 나머지는 컴포넌트로 가져오기
	transform = gameObject->GetComponent<Transform>();
	rigidbody = gameObject->GetComponent<Rigidbody>();
	spriteRenderer = gameObject->GetComponent<SpriteRenderer>();
}

void HonmunCollisionScript::Start()
{
	// 초기 설정
	started = true;
}

void HonmunCollisionScript::Update()
{
	// 쿨다운 처리
	if (reactionCooldown > 0)
	{
		reactionCooldown -= Time::GetDeltaTime();
	}

	// 낙하 속도 적용 (중력 외의 추가 속도)
	if (rigidbody && !rigidbody->isKinematic)
	{
		rigidbody->velocity.y -= fallingSpeed * 0.1f;
	}
}

void HonmunCollisionScript::OnCollisionEnter(ICollider* other, const ContactInfo& contact)
{
	// 쿨다운 중이면 무시
	if (reactionCooldown > 0 || isProcessingReaction) return;

	HonmunCollisionScript* otherScript = GetHonmunScript(other);
	if (!otherScript) return;

	// 연쇄반응 플래그 설정
	isProcessingReaction = true;
	otherScript->isProcessingReaction = true;

	// 타입별 반응 처리
	if (honmunType == otherScript->honmunType)
	{
		// 같은 타입끼리 충돌
		switch (honmunType)
		{
		case HonmunType::A: // Ignis - 합체
			HandleIgnisReaction(otherScript, contact);
			break;
		case HonmunType::B: // Umbra - 분열
			HandleUmbraReaction(otherScript, contact);
			break;
		case HonmunType::C: // Darkness - 흡인
			HandleDarknessReaction(otherScript, contact);
			break;
		case HonmunType::D: // Luna - 증발
			HandleLunaReaction(otherScript, contact);
			break;
		}
	}
	else
	{
		// 다른 타입끼리 충돌
		HandleMixedReaction(otherScript, contact);
	}

	// 쿨다운 설정
	reactionCooldown = 0.5f;
	isProcessingReaction = false;
	if (otherScript) otherScript->isProcessingReaction = false;
}

void HonmunCollisionScript::SetHonmunType(HonmunType type)
{
	honmunType = type;

	// 타입별 기본 속성 설정
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
	// A + A = 합체 (크기 10% 증가, 낙하속도 20% 감소)
	MergeWithOther(otherScript);
}

void HonmunCollisionScript::HandleUmbraReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact)
{
	// B + B = 분열 (각각 2개로 분리, 크기 30% 감소, 낙하속도 20% 증가)
	SplitIntoTwo();
	otherScript->SplitIntoTwo();
}

void HonmunCollisionScript::HandleDarknessReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact)
{
	// C + C = 흡인 (카메라 범위 내 적들을 충돌지점으로 끌어당기고 파괴)
	Vector2 collisionPoint = contact.point;
	AbsorbNearbyEnemies(collisionPoint);

	// 자신들도 파괴
	DestroyThis();
	otherScript->DestroyThis();
}

void HonmunCollisionScript::HandleLunaReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact)
{
	// D + any = 증발 (모든 충돌에서 파괴)
	DestroyThis();
	if (otherScript->honmunType == HonmunType::D)
	{
		otherScript->DestroyThis();
	}
}

void HonmunCollisionScript::HandleMixedReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact)
{
	// D가 포함된 충돌은 항상 D가 파괴됨
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

	// A&B 또는 B&A - 튕김
	if ((honmunType == HonmunType::A && otherScript->honmunType == HonmunType::B) ||
		(honmunType == HonmunType::B && otherScript->honmunType == HonmunType::A))
	{
		BounceAway(otherScript, contact);
	}
	// A&C 또는 C&A - 밀림
	else if ((honmunType == HonmunType::A && otherScript->honmunType == HonmunType::C) ||
		(honmunType == HonmunType::C && otherScript->honmunType == HonmunType::A))
	{
		PushSideways(otherScript);
	}
	// B&C 또는 C&B - 관통
	else if ((honmunType == HonmunType::B && otherScript->honmunType == HonmunType::C) ||
		(honmunType == HonmunType::C && otherScript->honmunType == HonmunType::B))
	{
		PassThrough(otherScript);
	}
}

void HonmunCollisionScript::MergeWithOther(HonmunCollisionScript* otherScript)
{
	// 크기 10% 증가
	UpdateSize(currentSize * 1.1f);

	// 낙하속도 20% 감소
	UpdateFallingSpeed(0.8f);

	// 상대방 제거
	otherScript->DestroyThis();
}

void HonmunCollisionScript::SplitIntoTwo()
{
	// 크기 30% 감소
	UpdateSize(currentSize * 0.7f);

	// 낙하속도 20% 증가
	UpdateFallingSpeed(1.2f);

	// 새로운 혼문 생성
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
	auto nearbyHonmuns = GetNearbyHonmuns(100.0f); // 100 픽셀 반경

	for (auto* honmunScript : nearbyHonmuns)
	{
		if (honmunScript == this) continue;

		// 카메라 범위 내에 있는지 확인
		if (honmunScript->IsInCameraView())
		{
			// 충돌지점으로 끌어당기기
			Vector2 direction = collisionPoint - honmunScript->transform->GetPosition();
			direction = direction.Normalized();

			// 밀리는 거리의 1/3만큼 이동
			Vector2 pullForce = direction * (pushDistance / 3.0f);
			honmunScript->rigidbody->AddImpulse(pullForce);

			// 일정 시간 후 파괴 (흡입 효과)
			honmunScript->reactionCooldown = 1.0f; // 1초 후 파괴되도록 설정
		}
	}
}

void HonmunCollisionScript::DestroyThis()
{
	if (gameObject)
	{
		gameObject->SetActive(false); // 또는 Destroy 함수 사용
	}
}

void HonmunCollisionScript::BounceAway(HonmunCollisionScript* otherScript, const ContactInfo& contact)
{
	// 진행방향과 반대로 튕겨냄
	Vector2 bounceDirection = -contact.normal;
	Vector2 bounceForce = bounceDirection * pushDistance;

	rigidbody->AddImpulse(bounceForce);
	otherScript->rigidbody->AddImpulse(-bounceForce);
}

void HonmunCollisionScript::PushSideways(HonmunCollisionScript* otherScript)
{
	// 좌우로 밀어냄
	Vector2 leftDirection(-1.0f, 0.0f);
	Vector2 rightDirection(1.0f, 0.0f);

	rigidbody->AddImpulse(leftDirection * pushDistance);
	otherScript->rigidbody->AddImpulse(rightDirection * pushDistance);
}

void HonmunCollisionScript::PassThrough(HonmunCollisionScript* otherScript)
{
	// 관통 - 물리적 충돌을 무시하지만 겹치지는 않게 함
	// 일시적으로 콜라이더를 트리거로 변경하거나 위치를 약간 조정
	Vector2 separationDirection = (transform->GetPosition() - otherScript->transform->GetPosition()).Normalized();
	Vector2 separationForce = separationDirection * 1.0f;

	rigidbody->AddImpulse(separationForce);
	otherScript->rigidbody->AddImpulse(-separationForce);
}

std::vector<HonmunCollisionScript*> HonmunCollisionScript::GetNearbyHonmuns(float radius)
{
	std::vector<HonmunCollisionScript*> nearbyHonmuns;

	// 현재 씬의 모든 게임오브젝트에서 HonmunCollisionScript를 가진 것들을 찾기
	// 이 부분은 씬 관리 시스템에 따라 구현 방법이 달라질 수 있습니다

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
		float scaleFactor = newSize / 10.0f; // 기본 크기를 10으로 가정
		transform->SetScale(scaleFactor, scaleFactor);
	}
}

void HonmunCollisionScript::UpdateFallingSpeed(float speedMultiplier)
{
	fallingSpeed *= speedMultiplier;
}

bool HonmunCollisionScript::IsInCameraView()
{
	// 카메라 범위 내에 있는지 확인하는 로직
	// 실제 구현은 카메라 시스템에 따라 달라질 수 있습니다
	return true; // 임시
}

HonmunCollisionScript* HonmunCollisionScript::GetHonmunScript(ICollider* collider)
{
	if (!collider || !collider->gameObject) return nullptr;

	return collider->gameObject->GetComponent<HonmunCollisionScript>();
}