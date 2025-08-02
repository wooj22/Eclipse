#pragma once
#include "HonmunCollisionBase.h"
#include "../Direct2D_EngineLib/ICollider.h"

class HonmunCollisionTypes
{
public:
    HonmunCollisionTypes() = default;
    ~HonmunCollisionTypes() = default;

    // Main collision processing
    void ProcessCollision(HonmunCollisionBase* script1, HonmunCollisionBase* script2, const ContactInfo& contact);

private:
    // Type-specific reactions
    void HandleIgnisReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript);     // A type
    void Handle2AReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript);        // A2 type (2A+2A)
    void HandleUmbraReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript);     // B type
    void HandleSmallBReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript);    // b type (split B)
    void HandleDarknessReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript);  // C type
    void HandleLunaReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript);      // D type

    // Mixed type reactions
    void HandleMixedReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript);

    // Helper methods
    bool ShouldProcessCollision(HonmunCollisionBase* script1, HonmunCollisionBase* script2);
    void ApplyHealthChange(HonmunCollisionBase* script, int healthChange);
    void ApplyCollisionForce(HonmunCollisionBase* script, HonmunCollisionBase* otherScript, float force);
    void ApplyOppositeForces(HonmunCollisionBase* script1, HonmunCollisionBase* script2, float force);
    void ApplyLeftRightPush(HonmunCollisionBase* script1, HonmunCollisionBase* script2, float force);
    void ApplyPenetration(HonmunCollisionBase* script1, HonmunCollisionBase* script2);
};