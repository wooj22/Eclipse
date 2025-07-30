#pragma once
#include <unordered_map>
#include "Component.h"
#include "Vector2.h"
#include "RaycastHit.h"

template<typename T>
T clamp(T value, T minVal, T maxVal)		
{
    if (value < minVal) return minVal;
    else if (value > maxVal) return maxVal;
    else return value;
}

/* ContactInfo : 콜라이더와 콜라이더의 충돌 정보 */
struct ContactInfo
{
	Vector2 point;   // 충돌 지점
	Vector2 normal;  // 충돌 법선벡터 : 밀려나야할 방향(상대 콜라이더가 밀어내는 방향)
	float depth;     // 침투 깊이
};

/* collider type : 콜라이더 종류 */
enum class ColliderType
{
	Box, Circle
};

/* collider interface*/
class ICollider : public Component
{
public:
	// default type
	ColliderType colliderType = ColliderType::Box;

	// collision, trigger
	bool isTrigger = false;             

	// bound
	float minX, maxX, minY, maxY;

	// collision data <충돌한 상대 콜라이더, 충돌정보>
	// Enter / Stay / Eixt
	std::unordered_map<ICollider*, ContactInfo> currentFrameCollisions;
	std::unordered_map<ICollider*, ContactInfo> lastFrameCollisions;

	// cycle
	virtual ~ICollider() = default;
	virtual void UpdateBounds() = 0;
	virtual bool isCollision(ICollider* other, ContactInfo& contact) = 0;
	virtual void FinalizeCollision() = 0;

	// ray cast hit
	virtual bool Raycast(const Ray& ray, float maxDistance, RaycastHit& hitInfo) = 0;

	// collision event
	virtual void OnCollisionEnter(ICollider* other, ContactInfo& contact) = 0;
	virtual void OnCollisionStay(ICollider* other, ContactInfo& contact) = 0;
	virtual void OnCollisionExit(ICollider* other, ContactInfo& contact) = 0;
	virtual void OnTriggerEnter(ICollider* other) = 0;
	virtual void OnTriggerStay(ICollider* other) = 0;
	virtual void OnTriggerExit(ICollider* other) = 0;

	// debug draw
	virtual void DebugColliderDraw() {}
};
