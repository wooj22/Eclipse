#pragma once
#include "../Direct2D_EngineLib/GameObject.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/ResourceManager.h"

enum class HonmunType
{
	A,
	B,
	C,
	D
};

class Honmun : public GameObject
{
public:
	// 기본 생성자 (CreateObject가 매개변수 없이 호출됨)
	Honmun();

	virtual void Awake() override;
	virtual void SceneStart() override;
	virtual void Update() override;
	virtual void Destroyed() override;

	// 혼문 타입 설정 및 위치 설정 함수들
	void SetHonmunType(HonmunType type);
	void SetPosition(float x, float y);

	// 충돌 효과를 위한 함수들
	void SetAlpha(float alpha);
	void ResetAlpha();

private:
	HonmunType honmunType;
	Transform* transform;
	SpriteRenderer* spriteRenderer;
	Rigidbody* rigidbody;
	CircleCollider* collider;

	// 각 타입별 리소스 경로
	std::string GetTexturePath();
	std::string GetSpriteName();
};