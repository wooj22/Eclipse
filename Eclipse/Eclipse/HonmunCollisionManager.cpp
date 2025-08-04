#include "HonmunCollisionManager.h"
#include "Aron_Scene.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/Camera.h"
#include <algorithm>
#include <cmath>

std::vector<HonmunCollisionBase*> HonmunCollisionManager::GetNearbyHonmuns(HonmunCollisionBase* script, float radius)
{
    std::vector<HonmunCollisionBase*> nearbyHonmuns;
    
    if (!script || !script->GetTransform()) {
        OutputDebugStringA("GetNearbyHonmuns: Invalid script or transform\n");
        return nearbyHonmuns;
    }
    
    // 동시 수정 중이면 빈 벡터 반환
    if (isUpdating) {
        OutputDebugStringA("GetNearbyHonmuns: Registry is updating, returning empty list\n");
        return nearbyHonmuns;
    }
    
    Vector2 centerPos = script->GetTransform()->GetPosition();
    
    // 안전한 복사본 생성으로 iterator 무효화 방지
    std::vector<HonmunCollisionBase*> registeredCopy = registeredHonmuns;
    
    // 복사본을 안전하게 순회
    for (auto* honmun : registeredCopy)
    {
        if (!IsValidHonmun(honmun) || honmun == script) continue;
        
        // transform 유효성 재검사
        if (!honmun->GetTransform()) continue;
        
        Vector2 honmunPos = honmun->GetTransform()->GetPosition();
        float distance = CalculateDistance(centerPos, honmunPos);
        
        if (distance <= radius)
        {
            nearbyHonmuns.push_back(honmun);
        }
    }
    
    char debugMsg[100];
    sprintf_s(debugMsg, "GetNearbyHonmuns: Found %zu nearby objects within %.1f radius\n", 
             nearbyHonmuns.size(), radius);
    OutputDebugStringA(debugMsg);
    
    return nearbyHonmuns;
}

bool HonmunCollisionManager::IsWithinCameraView(const Vector2& position)
{
    Vector2 cameraBounds = GetCameraBounds();
    
    // 카메라 영역 내부인지 확인 (여유분 추가)
    float margin = 100.0f;
    return position.x >= -cameraBounds.x - margin && 
           position.x <= cameraBounds.x + margin &&
           position.y >= -cameraBounds.y - margin && 
           position.y <= cameraBounds.y + margin;
}

float HonmunCollisionManager::CalculateDistance(const Vector2& pos1, const Vector2& pos2)
{
    float dx = pos1.x - pos2.x;
    float dy = pos1.y - pos2.y;
    return std::sqrt(dx * dx + dy * dy);
}

void HonmunCollisionManager::NotifyHonmunDestroyed(HonmunType type, float size)
{
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    if (aronScene)
    {
        int points = 1;
        
        // 타입별 기본 점수
        switch (type)
        {
        case HonmunType::A:
            points = (size > 15.0f) ? 3 : 1; // 2A는 3점
            break;
        case HonmunType::B:
            points = 1;
            break;
        case HonmunType::C:
            points = 2;
            break;
        case HonmunType::D:
            points = 2;
            break;
        }
        
        aronScene->AddScore(points);
        
        char debugMsg[100];
        sprintf_s(debugMsg, "Honmun destroyed - Type: %d, Size: %.2f, Points: %d\n", 
                 static_cast<int>(type), size, points);
        OutputDebugStringA(debugMsg);
    }
}

void HonmunCollisionManager::NotifyCollisionEvent(HonmunType type1, HonmunType type2)
{
    char debugMsg[150];
    sprintf_s(debugMsg, "Collision Event - Type1: %d, Type2: %d\n", 
             static_cast<int>(type1), static_cast<int>(type2));
    OutputDebugStringA(debugMsg);
}

void HonmunCollisionManager::RegisterHonmun(HonmunCollisionBase* script)
{
    if (!script) return;
    
    // 동시 수정 중이면 스킵
    if (isUpdating) {
        OutputDebugStringA("RegisterHonmun: Registry is updating, skipping registration\n");
        return;
    }
    
    // 중복 등록 방지
    auto it = std::find(registeredHonmuns.begin(), registeredHonmuns.end(), script);
    if (it == registeredHonmuns.end())
    {
        registeredHonmuns.push_back(script);
        
        char debugMsg[100];
        sprintf_s(debugMsg, "Honmun registered - Total count: %zu\n", registeredHonmuns.size());
        OutputDebugStringA(debugMsg);
    }
}

void HonmunCollisionManager::UnregisterHonmun(HonmunCollisionBase* script)
{
    RemoveFromRegistry(script);
}

void HonmunCollisionManager::CleanupDestroyedObjects()
{
    // 동시 수정 중이면 스킵
    if (isUpdating) {
        OutputDebugStringA("CleanupDestroyedObjects: Registry is updating, skipping cleanup\n");
        return;
    }
    
    // 안전한 정리를 위해 플래그 설정
    isUpdating = true;
    
    auto originalSize = registeredHonmuns.size();
    
    // 유효하지 않은 객체들 제거
    registeredHonmuns.erase(
        std::remove_if(registeredHonmuns.begin(), registeredHonmuns.end(),
            [this](HonmunCollisionBase* script) { return !IsValidHonmun(script); }),
        registeredHonmuns.end()
    );
    
    auto newSize = registeredHonmuns.size();
    if (newSize < originalSize) {
        char debugMsg[100];
        sprintf_s(debugMsg, "CleanupDestroyedObjects: Cleaned %zu invalid objects, remaining: %zu\n", 
                 originalSize - newSize, newSize);
        OutputDebugStringA(debugMsg);
    }
    
    // 플래그 해제
    isUpdating = false;
}

bool HonmunCollisionManager::IsOutOfBounds(const Vector2& position)
{
    Vector2 bounds = GetCameraBounds();
    float margin = 200.0f; // 화면 밖으로 너무 멀리 나가면 제거
    
    return position.x < -bounds.x - margin || 
           position.x > bounds.x + margin ||
           position.y < -bounds.y - margin || 
           position.y > bounds.y + margin;
}

Vector2 HonmunCollisionManager::GetCameraBounds()
{
    // 기본 카메라 영역 반환 (실제 카메라 시스템에 맞게 조정 필요)
    return Vector2(800.0f, 600.0f); // 1600x1200 해상도 기준
}

void HonmunCollisionManager::RemoveFromRegistry(HonmunCollisionBase* script)
{
    // 동시 수정 중이면 스킵
    if (isUpdating) {
        OutputDebugStringA("RemoveFromRegistry: Registry is updating, skipping removal\n");
        return;
    }
    
    // 안전한 제거를 위해 플래그 설정
    isUpdating = true;
    
    // 안전한 제거: erase-remove idiom 사용
    auto originalSize = registeredHonmuns.size();
    registeredHonmuns.erase(
        std::remove(registeredHonmuns.begin(), registeredHonmuns.end(), script),
        registeredHonmuns.end()
    );
    
    auto newSize = registeredHonmuns.size();
    if (newSize < originalSize) {
        char debugMsg[100];
        sprintf_s(debugMsg, "Honmun unregistered - Remaining count: %zu (removed %zu instances)\n", 
                 newSize, originalSize - newSize);
        OutputDebugStringA(debugMsg);
    }
    
    // 플래그 해제
    isUpdating = false;
}

bool HonmunCollisionManager::IsValidHonmun(HonmunCollisionBase* script)
{
    return script != nullptr && 
           script->GetTransform() != nullptr && 
           !script->IsMarkedForDestroy();
}