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
	B,
	C,
	D
};

class HonmunAFSM;
class HonmunAAnimatorController;
class HonmunBFSM;
class HonmunBAnimatorController;

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

private:
	HonmunType honmunType;
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	Rigidbody* rigidbody;
	CircleCollider* collider;
	Animator* animator;

	// FSM and AnimatorController for each type
	HonmunAFSM* honmunAFSM;
	HonmunAAnimatorController* honmunAAnimatorController;
	HonmunBFSM* honmunBFSM;
	HonmunBAnimatorController* honmunBAnimatorController;

	// 혼문 상태 변수들
	int hp = 2;                    // 체력 (기본값 2)
	float size = 1.0f;             // 크기 (기본값 1.0)
	bool isMerged = false;         // 합체 상태 (기본값 false)

	// �� Ÿ�Ժ� ���ҽ� ���
	std::string GetTexturePath();
	std::string GetSpriteName();
};