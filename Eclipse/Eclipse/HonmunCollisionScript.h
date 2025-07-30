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
	float fallingSpeed = 1.0f;
	float pushDistance = 10.0f;

	// ������� ó����
	bool isProcessingReaction = false;
	float reactionCooldown = 0.0f;

public:
	void Awake() override;
	void Start() override;
	void Update() override;

	// �浹 �̺�Ʈ
	void OnCollisionEnter(ICollider* other, const ContactInfo& contact) override;

	// ȥ�� Ÿ�� ����
	void SetHonmunType(HonmunType type);
	void SetHealth(int hp) { health = hp; }

private:
	// �� Ÿ�Ժ� �浹 ����
	void HandleIgnisReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact);     // A - ��ü
	void HandleUmbraReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact);     // B - �п�
	void HandleDarknessReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact);  // C - ����
	void HandleLunaReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact);      // D - ����

	// ȥ�� Ÿ�� �浹 ����
	void HandleMixedReaction(HonmunCollisionScript* otherScript, const ContactInfo& contact);

	// ��ƿ��Ƽ �Լ���
	void MergeWithOther(HonmunCollisionScript* otherScript);                    // ��ü
	void SplitIntoTwo();                                                        // �п�
	void AbsorbNearbyEnemies(const Vector2& collisionPoint);                    // ����
	void DestroyThis();                                                         // ����
	void BounceAway(HonmunCollisionScript* otherScript, const ContactInfo& contact);  // ƨ��
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