#pragma once
#include "HonmunCollisionBase.h"
#include "../Direct2D_EngineLib/Vector2.h"
#include "../Direct2D_EngineLib/ICollider.h"

class HonmunCollisionEffects
{
public:
    HonmunCollisionEffects() = default;
    ~HonmunCollisionEffects() = default;

    // Physical effects
    void MergeWithOther(HonmunCollisionBase* script, HonmunCollisionBase* otherScript);
    void SplitIntoTwo(HonmunCollisionBase* script);
    void CreateSplitObjects(HonmunCollisionBase* script, int count);
    void CreateSplitObjectsWithCollision(HonmunCollisionBase* script, int count, HonmunCollisionBase* otherScript);
    void AbsorbNearbyEnemies(HonmunCollisionBase* script, const Vector2& collisionPoint);
    void BounceAway(HonmunCollisionBase* script, HonmunCollisionBase* otherScript, const ContactInfo& contact);
    void BounceAwayKinematic(HonmunCollisionBase* script, HonmunCollisionBase* otherScript);
    void PushSideways(HonmunCollisionBase* script, HonmunCollisionBase* otherScript);
    void PassThrough(HonmunCollisionBase* script, HonmunCollisionBase* otherScript);

private:
    // Helper methods
    Vector2 GetRandomDirection();
    void UpdateSize(HonmunCollisionBase* script, float newSize);
    void UpdateFallingSpeed(HonmunCollisionBase* script, float speedMultiplier);
    void ApplyPhysicsTransition(HonmunCollisionBase* script);
};