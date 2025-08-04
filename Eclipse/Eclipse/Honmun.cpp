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

Honmun::Honmun() : GameObject("Honmun_A"), honmunType(HonmunType::A), honmunFSM(nullptr)
{
	transform = AddComponent<Transform>();
	spriteRenderer = AddComponent<SpriteRenderer>();
	rigidbody = AddComponent<Rigidbody>();
	collider = AddComponent<CircleCollider>();
	animator = AddComponent<Animator>();

	// 새로운 충돌 시스템 추가
	auto* collisionScript = AddComponent<HonmunCollisionBase>();
	
	// 충돌 시스템 디버그 확인
	char setupMsg[100];
	sprintf_s(setupMsg, "Honmun created with collision system: %s\n", 
	         collisionScript ? "SUCCESS" : "FAILED");
	OutputDebugStringA(setupMsg);
	
	// 통합 FSM 추가
	honmunFSM = AddComponent<HonmunFSM>();
}

void Honmun::Awake()
{
	// 애니메이션 시스템 활성화 - 통합 FSM 사용
	SetupAnimationSystem();

	// 물리 설정 - 중력 제거하고 키네마틱으로 변경
	rigidbody->useGravity = false;  // 중력 비활성화
	rigidbody->isKinematic = true;  // 키네마틱 모드로 변경
	
	// 타입별 낙하 속도 설정 (키네마틱 모드로 중력 사용 안함)
	// SetFallingSpeedByType(); // 중력 사용 안하므로 주석처리
	
	// 타입에 따른 이름 설정
	UpdateNameByType();

	// 스프라이트 크기를 원본 크기로 설정
	transform->SetScale(1.0f, 1.0f);
	
	// 충돌 감지 최적화를 위한 콜라이더 설정
	SetupColliderForType();
	
	// 리지드바디 키네마틱 설정 유지
	if (rigidbody)
	{
		rigidbody->isKinematic = true;  // 키네마틱 모드 유지
		rigidbody->useGravity = false;  // 중력 완전 비활성화
		rigidbody->drag = 0.0f;         // 드래그 제거
	}
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
	// 안전한 타입 변경 추적을 위한 디버그 로그
	std::string safeName = "UNKNOWN";
	try {
		if (!name.empty()) {
			safeName = name;
		}
	} catch (...) {
		OutputDebugStringA("WARNING: Exception accessing name in SetHonmunType\n");
		safeName = "CORRUPTED_NAME";
	}
	
	char debugMsg[150];
	sprintf_s(debugMsg, "SetHonmunType called: %s changing from %d to %d\n", 
		safeName.c_str(), static_cast<int>(honmunType), static_cast<int>(type));
	OutputDebugStringA(debugMsg);
	
	honmunType = type;
	
	// 타입에 따른 이름 업데이트
	UpdateNameByType();

	// 타입별 기본 체력 설정 (HP가 1이면 2A 상태이므로 유지)
	if (hp != 1) // 2A 상태(HP=1)가 아닌 경우에만 기본 HP 설정
	{
		switch (type)
		{
		case HonmunType::A:
			hp = 3; // A타입: 3 HP
			break;
		case HonmunType::B:
			hp = 2; // B타입: 2 HP
			break;
		case HonmunType::C:
			hp = 3; // C타입: 3 HP로 수정
			break;
		case HonmunType::D:
			hp = 1; // D타입: 1 HP
			break;
		case HonmunType::A2:
			hp = 1; // A2타입: 1 HP (2A)
			break;
		case HonmunType::b:
			hp = 1; // b타입: 1 HP (B 분열 조각)
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
	
	// 타입 변경 시 낙하 속도도 재설정
	SetFallingSpeedByType();
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
	case HonmunType::A2: return "../Resource/Aron/Honmun_a.png"; // A2도 A 텍스처 사용
	case HonmunType::b: return "../Resource/Aron/Honmun_b.png"; // 소문자 b도 B 텍스처 사용
	default: return "../Resource/Aron/Honmun_a.png";
	}
}

std::string Honmun::GetSpriteName()
{
	switch (honmunType)
	{
	case HonmunType::A: return "Honmun_A_0";  // 첫 번째 프레임 사용
	case HonmunType::B: return "Honmun_B_0";  // 첫 번째 프레임 사용
	case HonmunType::C: return "Honmun_C_0";  // 첫 번째 프레임 사용
	case HonmunType::D: return "Honmun_D_0";  // 첫 번째 프레임 사용
	case HonmunType::A2: return "Honmun_A2_0"; // A2 전용 스프라이트
	case HonmunType::b: return "Honmun_b_0";  // 소문자 b 전용 스프라이트
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
	if (!collider) {
		OutputDebugStringA("ERROR: Collider is null in SetupColliderForType!\n");
		return;
	}
	
	collider->isTrigger = true; // 트리거 모드 보장
	
	// 충돌 감지 최적화를 위해 콜라이더 크기 증가 및 위치 조정
	switch (honmunType)
	{
	case HonmunType::A:
		collider->radius = 40.0f; // 충돌 감지 개선을 위해 크기 증가
		collider->offset.y = -8.0f; // 위치 조정
		break;
	case HonmunType::B:
		collider->radius = 34.0f; // 원래 크기로 복원
		collider->offset.y = -23.0f; // 원래 위치로 복원 (아래쪽)
		break;
	case HonmunType::C:
		collider->radius = 35.0f; // 충돌 감지 개선
		collider->offset.y = -12.0f;
		break;
	case HonmunType::D:
		collider->radius = 32.0f; // 충돌 감지 개선
		collider->offset.y = -5.0f;
		break;
	case HonmunType::A2:
		collider->radius = 42.0f; // A2는 더 크게 (충돌 감지 우선)
		collider->offset.y = -8.0f;
		break;
	case HonmunType::b:
		collider->radius = 35.0f; // b 조각 충돌 감지 개선
		collider->offset.y = -12.0f;
		break;
	default:
		collider->radius = 40.0f;
		collider->offset.y = -8.0f;
		break;
	}
	
	// 충돌 감지 성능 최적화 메시지
	char colliderMsg[100];
	sprintf_s(colliderMsg, "Collider optimized: Type %d, radius=%.1f, offset=%.1f\n", 
	         static_cast<int>(honmunType), collider->radius, collider->offset.y);
	OutputDebugStringA(colliderMsg);
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
	case HonmunType::A2: return "../Resource/Aron/Data/SpriteSheet/Honmun_A_sprites.json"; // A2는 A 사용
	case HonmunType::b: return "../Resource/Aron/Data/SpriteSheet/Honmun_B_sprites.json"; // b는 B 사용
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
	case HonmunType::A2: return "../Resource/Aron/Data/AnimationClip/Honmun_A_Idle_AniClip.json"; // A2는 A 사용
	case HonmunType::b: return "../Resource/Aron/Data/AnimationClip/Honmun_B_Idle_AniClip.json"; // b는 B 사용
	default: return "../Resource/Aron/Data/AnimationClip/Honmun_A_Idle_AniClip.json";
	}
}

void Honmun::SetFallingSpeedByType()
{
	if (!rigidbody) return;
	
	switch (honmunType)
	{
	case HonmunType::A:
	case HonmunType::B:
	case HonmunType::C:
		rigidbody->gravityScale = 0.5f; // A, B, C 타입: 중력 약화 (너무 세서 조정)
		break;
	case HonmunType::b:
		rigidbody->gravityScale = 0.6f; // b 조각: 기본보다 20% 증가 (낙하속도 20% 증가)
		break;
	case HonmunType::A2:
		rigidbody->gravityScale = 0.4f; // A2 타입: 기본보다 20% 감소 (낙하속도 20% 감소)
		break;
	case HonmunType::D:
		rigidbody->gravityScale = 1.0f; // D 타입: 2배였으나 중력 약화로 1.0f로 조정 (상대적으로 2배)
		break;
	default:
		rigidbody->gravityScale = 1.0f; // 기본값
		break;
	}
	
	// 디버그 로그
	char speedMsg[100];
	sprintf_s(speedMsg, "Honmun type %d gravity set to %.1f\n", 
		static_cast<int>(honmunType), rigidbody->gravityScale);
	OutputDebugStringA(speedMsg);
}

void Honmun::UpdateNameByType()
{
	switch (honmunType)
	{
	case HonmunType::A:
		name = "Honmun_A";
		break;
	case HonmunType::B:
		name = "Honmun_B";
		break;
	case HonmunType::C:
		name = "Honmun_C";
		break;
	case HonmunType::D:
		name = "Honmun_D";
		break;
	case HonmunType::A2:
		name = "Honmun_A2";
		break;
	case HonmunType::b:
		name = "Honmun_b";
		break;
	default:
		name = "Honmun_Unknown";
		break;
	}
}