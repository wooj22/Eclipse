#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "Honmun.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"
#include "../Direct2D_EngineLib/Vector2.h"
#include <vector>

// Forward declarations
class HonmunCollisionTypes;
class HonmunCollisionEffects;
class HonmunCollisionManager;

class HonmunCollisionBase : public Script
{
protected:
	// Core components
	Honmun* honmun = nullptr;
	Transform* transform = nullptr;
	Rigidbody* rigidbody = nullptr;
	SpriteRenderer* spriteRenderer = nullptr;

	// 혼문 속성
	HonmunType honmunType;
	int health = 3;
	float currentSize = 10.0f;
	float targetSize = -1.0f;
	float fallingSpeed = 1.0f;
	float pushDistance = 150.0f;
	bool isSplitFragment = false;
	
	// 분열 조각 물리 전환용
	float splitPhysicsTimer = 0.0f;
	bool needsPhysicsTransition = false;

	// 반응 처리
	bool isProcessingReaction = false;
	float reactionCooldown = 0.0f;
	
	// 충돌 후 관성 효과
	Vector2 currentVelocity;
	float friction = 0.95f;
	float minVelocity = 0.1f;

	// Helper classes
	HonmunCollisionTypes* collisionTypes = nullptr;
	HonmunCollisionEffects* collisionEffects = nullptr;
	HonmunCollisionManager* collisionManager = nullptr;

public:
	HonmunCollisionBase();
	virtual ~HonmunCollisionBase();

	// Script lifecycle
	void Awake() override;
	void Start() override;
	void Update() override;

	// Collision events
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;
	void OnCollisionEnter(ICollider* other, const ContactInfo& contact) override;

	// Public interface
	void SetHonmunType(HonmunType type);
	void SetHealth(int hp) { health = hp; }
	void DestroyThis();

	// Getters for helper classes
	HonmunCollisionTypes* GetCollisionTypes() const { return collisionTypes; }
	HonmunCollisionEffects* GetCollisionEffects() const { return collisionEffects; }
	HonmunCollisionManager* GetCollisionManager() const { return collisionManager; }

	// Getters for components
	Honmun* GetHonmun() const { return honmun; }
	Transform* GetTransform() const { return transform; }
	Rigidbody* GetRigidbody() const { return rigidbody; }
	SpriteRenderer* GetSpriteRenderer() const { return spriteRenderer; }
	
	// Getters for properties
	HonmunType GetHonmunType() const { return honmunType; }
	int GetHealth() const { return health; }
	float GetCurrentSize() const { return currentSize; }
	bool IsSplitFragment() const { return isSplitFragment; }
	bool IsProcessingReaction() const { return isProcessingReaction; }
	
	// Setters
	void SetCurrentSize(float size) { currentSize = size; }
	void SetSplitFragment(bool isFragment) { isSplitFragment = isFragment; }
	void SetProcessingReaction(bool processing) { isProcessingReaction = processing; }
	
	// Status checks
	bool IsMarkedForDestroy() const { return markedForDestroy; }

protected:
	bool markedForDestroy = false;

private:
	void InitializeHelperClasses();
	void CleanupHelperClasses();
};