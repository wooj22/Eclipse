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
	void SetPosition(float x, float y);
	void AdjustSpritePosition(); // 스프라이트 위치 조정 함수

	// �浹 ȿ���� ���� �Լ���
	void SetAlpha(float alpha);
	void ResetAlpha();

private:
	HonmunType honmunType;
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	Rigidbody* rigidbody;
	CircleCollider* collider;
	Animator* animator;

	// FSM and AnimatorController for Honmun A
	HonmunAFSM* honmunAFSM;
	HonmunAAnimatorController* honmunAAnimatorController;

	// �� Ÿ�Ժ� ���ҽ� ���
	std::string GetTexturePath();
	std::string GetSpriteName();
};