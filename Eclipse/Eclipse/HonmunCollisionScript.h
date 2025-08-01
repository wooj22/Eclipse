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

	// ȥ�� �Ӽ�
	HonmunType honmunType;
	int health = 3;
	float currentSize = 10.0f;
	float targetSize = -1.0f;  // 목표 크기 (-1이면 설정 안됨)
	float fallingSpeed = 1.0f;
	float pushDistance = 150.0f;  // \ucda9\ub3bc \ud798 \uc54c\uae4c\uae30 \ucef4\uc149\uc5d0 \ub9de\uac8c \uc99d\uac00
	bool isSplitFragment = false;  // \ubd84\ud574\ub41c \uc870\uac01\uc778\uc9c0 \ud45c\uc2dc
	
	// 분열 조각 물리 전환용
	float splitPhysicsTimer = 0.0f;  // 분열 후 물리 전환 타이머
	bool needsPhysicsTransition = false;  // 물리 전환 필요 여부

	// ������� ó����
	bool isProcessingReaction = false;
	float reactionCooldown = 0.0f;
	
	// \ucda9\ub3cc \ud6c4 \uad00\uc131 \ud6a8\uacfc\ub97c \uc704\ud55c \ubcc0\uc218
	Vector2 currentVelocity;  // \uae30\ubcf8 \uc0dd\uc131\uc790 \uc0ac\uc6a9
	float friction = 0.95f;  // \ub9c8\ucc30\ub825 (\uac10\uc18d \uc815\ub3c4)
	float minVelocity = 0.1f;  // \ucd5c\uc18c \uc18d\ub3c4 (\uc774\ud558\uc5d0\uc11c \uc815\uc9c0)

public:
	void Awake() override;
	void Start() override;
	void Update() override;

	// 트리거 이벤트
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;

	// ȥ�� Ÿ�� ����
	void SetHonmunType(HonmunType type);
	void SetHealth(int hp) { health = hp; }
	
	// 공개 함수들
	void DestroyThis();  // 혼문 파괴 (외부에서 호출 가능)

private:
	// \uac01 \ud0c0\uc785\ubcc4 \ucda9\ub3cc \ucc98\ub9ac - \ubaa8\ub4e0 \ud0c0\uc785 \uccb4\ub825 \uac10\uc18c \uc2dc\uc2a4\ud15c
	void HandleIgnisReaction(HonmunCollisionScript* otherScript);     // A - \uccb4\ub825 \uac10\uc18c
	void HandleUmbraReaction(HonmunCollisionScript* otherScript);     // B - \uccb4\ub825 \uac10\uc18c
	void HandleDarknessReaction(HonmunCollisionScript* otherScript);  // C - \uccb4\ub825 \uac10\uc18c
	void HandleLunaReaction(HonmunCollisionScript* otherScript);      // D - \uccb4\ub825 \uac10\uc18c

	// ȥ�� Ÿ�� �浹 ����
	void HandleMixedReaction(HonmunCollisionScript* otherScript);

	// ��ƿ��Ƽ �Լ���
	void MergeWithOther(HonmunCollisionScript* otherScript);                    // ��ü
	void SplitIntoTwo();                                                        // 분열
	void CreateSplitObjects(int count);                                         // 다중 분열
	void CreateSplitObjectsWithCollision(int count, HonmunCollisionScript* otherScript); // 충돌 기반 분열
	void AbsorbNearbyEnemies(const Vector2& collisionPoint);                    // ����
	void BounceAway(HonmunCollisionScript* otherScript, const ContactInfo& contact);  // ƨ��
	void BounceAwayKinematic(HonmunCollisionScript* otherScript);  // \ud0a4\ub124\ub9c8\ud2f1 \ud295\uae40
	void PushSideways(HonmunCollisionScript* otherScript);                      // �и�
	void PassThrough(HonmunCollisionScript* otherScript);                       // ����

	// ���� �Լ���
	std::vector<HonmunCollisionScript*> GetNearbyHonmuns(float radius);
	Vector2 GetRandomDirection();
	void UpdateSize(float newSize);
	void UpdateFallingSpeed(float speedMultiplier);
	bool IsInCameraView();
	HonmunCollisionScript* GetHonmunScript(ICollider* collider);
};