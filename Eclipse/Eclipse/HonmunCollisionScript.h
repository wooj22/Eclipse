#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "Honmun.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Camera.h"
#include "../Direct2D_EngineLib/Vector2.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include <vector>

class HonmunCollisionScript : public Script
{
private:
	Honmun* honmun = nullptr;
	Transform* transform = nullptr;
	Rigidbody* rigidbody = nullptr;
	SpriteRenderer* spriteRenderer = nullptr;

	// 혼문 속성
	HonmunType honmunType;
	int health = 3;
	float currentSize = 10.0f;
	float fallingSpeed = 1.0f;
	float pushDistance = 10.0f;
	float chainEffectRadius = 50.0f; // 연쇄반응 반경

	// 연쇄반응 처리용
	bool isProcessingReaction = false;
	float reactionCooldown = 0.0f;
	bool isChainable = true; // 연쇄반응 가능 여부

public:
	void Awake() override;
	void Start() override;
	void Update() override;

	// 충돌 이벤트
	void OnCollisionEnter(ICollider* other, const ContactInfo& contact) override;

	// 혼문 타입 설정
	void SetHonmunType(HonmunType type);
	void SetHealth(int hp) { health = hp; }

private:
	// 같은 타입별 충돌 처리
	void HandleIgnisReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact);     // A - 합체
	void HandleUmbraReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact);     // B - 분열
	void HandleDarknessReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact);  // C - 흡인
	void HandleLunaReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact);      // D - 증발

	// 혼합 타입 충돌 처리
	void HandleMixedReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact);

	// 액션유틸리티 함수들
	void MergeWithOther(HonmunCollisionScript* otherScript);                    // 합체
	void SplitIntoTwo();                                                        // 분열 (기본)
	void CreateSplitHonmuns(const Vector2& position);                           // B 타입 분열시 2개 생성
	void AbsorbNearbyEnemies(const Vector2& collisionPoint);                    // 흡인
	void DestroyThis();                                                         // 파괴
	void BounceAway(HonmunCollisionScript* otherScript, const ContactInfo& contact);  // 튕김
	void PushSideways(HonmunCollisionScript* otherScript);                      // 밀림
	void PassThrough(HonmunCollisionScript* otherScript);                       // 관통

	// 도움 함수들
	std::vector<HonmunCollisionScript*> GetNearbyHonmuns(float radius);
	Vector2 GetRandomDirection();
	void UpdateSize(float newSize);
	void UpdateFallingSpeed(float speedMultiplier);
	bool IsInCameraView();
	HonmunCollisionScript* GetHonmunScript(ICollider* collider);
	void TriggerChainReaction(); // 연쇄반응 유발
};