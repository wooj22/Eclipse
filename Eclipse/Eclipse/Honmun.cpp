#include "Honmun.h"
#include "HonmunCollisionBase.h"
#include "HonmunFSM.h"
#include "HonmunAAnimatorController.h"
#include "HonmunBAnimatorController.h"
#include "HonmunCAnimatorController.h"
#include "HonmunDAnimatorController.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include <iostream>
#include <exception>

Honmun::Honmun() : GameObject("Honmun"), honmunType(HonmunType::A), honmunFSM(nullptr)
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
	rigidbody = AddComponent<Rigidbody>();
	collider = AddComponent<CircleCollider>();
	animator = AddComponent<Animator>();

	// 새로운 충돌 시스템 추가
	auto* collisionScript = AddComponent<HonmunCollisionBase>();
	
	// 통합 FSM 추가
	honmunFSM = AddComponent<HonmunFSM>();
}

void Honmun::Awake()
{
	// 애니메이션 시스템 활성화 - 통합 FSM 사용
	SetupAnimationSystem();

	// 물리 설정 - 충돌 테스트용 (중력 비활성화)
	rigidbody->useGravity = false;  // 중력 완전 비활성화
	rigidbody->isKinematic = true;  // 키네마틱 모드

	// 스프라이트 크기를 원본 크기로 설정
	transform->SetScale(1.0f, 1.0f);
	
	// 타입별 콜라이더 설정
	SetupColliderForType();
}

void Honmun::SceneStart()
{
	// 씬 시작 시 충돌 스크립트에 타입 설정
	auto* collisionScript = GetComponent<HonmunCollisionBase>();
	if (collisionScript)
	{
		collisionScript->SetHonmunType(honmunType);
	}
	
	// FSM에도 타입 설정
	if (honmunFSM)
	{
		honmunFSM->SetHonmunType(honmunType);
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
	// 소멸자 - 필요시 정리 작업
}

void Honmun::Destroyed()
{
	// ������Ʈ �ı� �� ���� �۾�
}

void Honmun::SetHonmunType(HonmunType type)
{
	honmunType = type;

	// 타입별 기본 체력 설정 (HP가 1이면 2A 상태이므로 유지)
	if (hp != 1) // 2A 상태(HP=1)가 아닌 경우에만 기본 HP 설정
	{
		switch (type)
		{
		case HonmunType::A:
			hp = 3; // A타입: 3 HP
			break;
		case HonmunType::A2:
			hp = 1; // A2타입: 1 HP (2A)
			break;
		case HonmunType::B:
			hp = 2; // B타입: 2 HP
			break;
		case HonmunType::b:
			hp = 1; // b타입: 1 HP (B 분열 조각)
			break;
		case HonmunType::C:
			hp = 3; // C타입: 3 HP로 수정
			break;
		case HonmunType::D:
			hp = 1; // D타입: 1 HP
			break;
		default:
			hp = 3;
			break;
		}
	}

	// 애니메이션 시스템 재설정
	SetupAnimationSystem();

	// 타입 변경 시 콜라이더 재설정
	SetupColliderForType();
	
	// 충돌 스크립트에 타입 설정
	auto* collisionScript = GetComponent<HonmunCollisionBase>();
	if (collisionScript)
	{
		collisionScript->SetHonmunType(type);
	}
	
	// FSM에 타입 설정
	if (honmunFSM)
	{
		honmunFSM->SetHonmunType(type);
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
	case HonmunType::A2: return "../Resource/Aron/Honmun_a.png"; // A2도 A 텍스처 사용
	case HonmunType::B: return "../Resource/Aron/Honmun_b.png";
	case HonmunType::b: return "../Resource/Aron/Honmun_b.png"; // 소문자 b도 B 텍스처 사용
	case HonmunType::C: return "../Resource/Aron/Honmun_c.png";
	case HonmunType::D: return "../Resource/Aron/Honmun_d.png";
	default: return "../Resource/Aron/Honmun_a.png";
	}
}

std::string Honmun::GetSpriteName()
{
	switch (honmunType)
	{
	case HonmunType::A: return "Honmun_A_0";  // 첫 번째 프레임 사용
	case HonmunType::A2: return "Honmun_A2_0"; // A2 전용 스프라이트
	case HonmunType::B: return "Honmun_B_0";  // 첫 번째 프레임 사용
	case HonmunType::b: return "Honmun_b_0";  // 소문자 b 전용 스프라이트
	case HonmunType::C: return "Honmun_C_0";  // 첫 번째 프레임 사용
	case HonmunType::D: return "Honmun_D_0";  // 첫 번째 프레임 사용
	default: return "Honmun_A_0";
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
		collider->radius = 35.0f; // 고정 크기로 변경
		collider->offset.y = -11.0f; // A타입은 살짝 아래로
		break;
	case HonmunType::A2:
		collider->radius = 38.0f; // A2는 조금 더 크게 (10% 증가)
		collider->offset.y = -11.0f; // A와 동일한 위치
		break;
	case HonmunType::B:
		collider->radius = 34.0f; // 고정 크기로 변경
		collider->offset.y = -23.0f; // B타입은 더 아래로
		break;
	case HonmunType::b:
		collider->radius = 24.0f; // 소문자 b는 더 작게 (30% 감소)
		collider->offset.y = -23.0f; // B와 동일한 위치
		break;
	case HonmunType::C:
		collider->radius = 30.0f; // 고정 크기로 변경
		collider->offset.y = -15.0f; // C타입 위치 조정
		break;
	case HonmunType::D:
		collider->radius = 26.0f; // 고정 크기로 변경
		collider->offset.y = -2.0f; // D타입 위치 조정
		break;
	default:
		collider->radius = 35.0f; // 기본값도 고정 크기
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

void Honmun::SetupAnimationSystem()
{
	// JSON 파싱 에러 방지를 위해 안전한 정적 스프라이트만 사용
	char debugMsg[100];
	sprintf_s(debugMsg, "Loading static sprite for type %d\n", static_cast<int>(honmunType));
	OutputDebugStringA(debugMsg);
	
	auto texture = ResourceManager::Get().CreateTexture2D(GetTexturePath());
	if (spriteRenderer && texture) 
	{
		// 스프라이트 시트에서 첫 번째 프레임의 영역 계산 (128x128 크기)
		D2D1_RECT_F firstFrameRect;
		firstFrameRect.left = 0.0f;      // 첫 번째 프레임 X 시작
		firstFrameRect.top = 0.0f;       // 첫 번째 프레임 Y 시작  
		firstFrameRect.right = 128.0f;   // 프레임 폭
		firstFrameRect.bottom = 128.0f;  // 프레임 높이
		
		// 피벗 포인트 (중앙)
		D2D1_POINT_2F pivotPoint = { 0.5f, 0.5f };
		
		// 첫 번째 프레임만 잘라서 스프라이트 생성
		spriteRenderer->sprite = ResourceManager::Get().CreateSprite(texture, GetSpriteName(), firstFrameRect, pivotPoint);
		
		sprintf_s(debugMsg, "Static sprite loaded for type %d (128x128)\n", static_cast<int>(honmunType));
		OutputDebugStringA(debugMsg);
	}
	else
	{
		OutputDebugStringA("Failed to load texture or spriteRenderer is null\n");
	}
}

std::string Honmun::GetSpriteSheetPath()
{
	switch (honmunType)
	{
	case HonmunType::A: return "../Resource/Aron/Data/SpriteSheet/Honmun_A_sprites.json";
	case HonmunType::B: return "../Resource/Aron/Data/SpriteSheet/Honmun_B_sprites.json";
	case HonmunType::C: return "../Resource/Aron/Data/SpriteSheet/Honmun_C_sprites.json";
	case HonmunType::D: return "../Resource/Aron/Data/SpriteSheet/Honmun_D_sprites.json";
	default: return "../Resource/Aron/Data/SpriteSheet/Honmun_A_sprites.json";
	}
}

std::string Honmun::GetAnimationClipPath()
{
	switch (honmunType)
	{
	case HonmunType::A: return "../Resource/Aron/Data/AnimationClip/Honmun_A_Idle_AniClip.json";
	case HonmunType::B: return "../Resource/Aron/Data/AnimationClip/Honmun_B_Idle_AniClip.json";
	case HonmunType::C: return "../Resource/Aron/Data/AnimationClip/Honmun_C_Idle_AniClip.json";
	case HonmunType::D: return "../Resource/Aron/Data/AnimationClip/Honmun_D_Idle_AniClip.json";
	default: return "../Resource/Aron/Data/AnimationClip/Honmun_A_Idle_AniClip.json";
	}
}