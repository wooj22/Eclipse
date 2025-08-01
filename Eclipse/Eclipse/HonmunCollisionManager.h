#pragma once
#include "HonmunCollisionBase.h"
#include "../Direct2D_EngineLib/Vector2.h"
#include <vector>

class HonmunCollisionManager
{
public:
    HonmunCollisionManager() = default;
    ~HonmunCollisionManager() = default;

    // Utility functions
    std::vector<HonmunCollisionBase*> GetNearbyHonmuns(HonmunCollisionBase* script, float radius);
    bool IsWithinCameraView(const Vector2& position);
    float CalculateDistance(const Vector2& pos1, const Vector2& pos2);
    
    // Score and data management
    void NotifyHonmunDestroyed(HonmunType type, float size);
    void NotifyCollisionEvent(HonmunType type1, HonmunType type2);
    
    // Object management
    void RegisterHonmun(HonmunCollisionBase* script);
    void UnregisterHonmun(HonmunCollisionBase* script);
    void CleanupDestroyedObjects();
    
    // Camera and bounds checking
    bool IsOutOfBounds(const Vector2& position);
    Vector2 GetCameraBounds();

private:
    std::vector<HonmunCollisionBase*> registeredHonmuns;
    
    // Helper methods
    void RemoveFromRegistry(HonmunCollisionBase* script);
    bool IsValidHonmun(HonmunCollisionBase* script);
};