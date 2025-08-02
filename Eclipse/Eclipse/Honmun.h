#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Animator.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

enum class HonmunType
{
	A,
	A2,  // 2A (A+A 합체 결과)
	B,
	b,   // B 분열 조각
	C,
	D
};

class HonmunFSM;

class Honmun : public GameObject
{
public:
	// �⺻ ������ (CreateObject�� �Ű����� ���� ȣ���)
	Honmun();
	virtual ~Honmun();

	virtual void Awake() override;
	virtual void SceneStart() override;
	virtual void Update() override;
	virtual void Destroyed() override;

	// ȥ�� Ÿ�� ���� �� ��ġ ���� �Լ���
	void SetHonmunType(HonmunType type);
	HonmunType GetHonmunType() const { return honmunType; }
	void SetPosition(float x, float y);
	void AdjustSpritePosition(); // 스프라이트 위치 조정 함수
	void SetupColliderForType(); // 타입별 콜라이더 설정 함수

	// 체력 관련
	int GetHP() const { return hp; }
	void SetHP(int newHP) { hp = newHP; }

	// 크기 관련
	float GetSize() const { return size; }
	void SetSize(float newSize);

	// 합체/분열 관련 상태
	bool IsMerged() const { return isMerged; }
	void SetMerged(bool merged) { isMerged = merged; }

	// 충돌 효과를 위한 함수들
	void SetAlpha(float alpha);
	void ResetAlpha();
	
	// 타입별 리소스 경로 (FSM에서 접근 가능하도록 public)
	std::string GetTexturePath();
	std::string GetSpriteName();

private:
	HonmunType honmunType;
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	Rigidbody* rigidbody;
	CircleCollider* collider;
	Animator* animator;

	// 통합 FSM
	HonmunFSM* honmunFSM;

	// 혼문 상태 변수들
	int hp = 3;                    // 체력 (기본값 3 - A타입 기준)
	float size = 1.0f;             // 크기 (기본값 1.0로 복원)
	bool isMerged = false;         // 합체 상태 (기본값 false)

	// 타입별 리소스 경로 (private으로 이동된 나머지)
	std::string GetSpriteSheetPath();
	std::string GetAnimationClipPath();
	
	// 애니메이션 시스템 설정
	void SetupAnimationSystem();
};