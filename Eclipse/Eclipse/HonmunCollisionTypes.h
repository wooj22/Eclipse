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
    
    // 기획서 기반 새로운 헬퍼 메소드들
    int GetDestructionScore(HonmunType type, int hp);
    int GetMixedCollisionScore(HonmunType type1, int hp1, HonmunType type2, int hp2);
    void ApplyPhysicalEffect(HonmunCollisionBase* script, HonmunCollisionBase* otherScript, HonmunType type1, HonmunType type2);
    void HandleDMixedCollision(HonmunCollisionBase* script, HonmunCollisionBase* otherScript, class Aron_Scene* aronScene);
    void HandleDestruction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript, 
                          bool destroyScript, bool destroyOther, class Aron_Scene* aronScene);
};