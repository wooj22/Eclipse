#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <random>
#include <mutex>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <cstdio>
#include <string>
#include <windows.h>

// 프로젝트 헤더들
#include "Aron_Scene.h"
#include "GameManager.h"
#include "HonmunCollisionBase.h"
#include "Honmun.h"
#include "GameObject.h"

// 🚨 크래시 수정: 안전한 객체 파괴 시스템

// === 문제 ===
// DestroyThis() 호출 시 객체는 파괴되지만
// 여러 벡터에서 여전히 참조하고 있어서 댕글링 포인터 발생

// === 해결책 ===
// 1. 즉시 파괴하지 말고 "파괴 예약" 시스템 사용
// 2. 모든 벡터에서 안전하게 제거 후 파괴
// 3. 파괴된 객체 접근 시 null 체크 강화

// HonmunCollisionBase.cpp 수정
void HonmunCollisionBase::DestroyThis()
{
    if (markedForDestroy) return; // 이미 파괴 표시됨
    
    markedForDestroy = true;
    
    char debugMsg[100];
    sprintf_s(debugMsg, "DestroyThis called for %s - MARKED for destruction\n", 
              gameObject ? gameObject->name.c_str() : "null");
    OutputDebugStringA(debugMsg);
    
    // 🔥 FIXED: 즉시 파괴하지 말고 비활성화만 하기
    if (honmun && honmun->IsActive())
    {
        honmun->SetActive(false);  // 즉시 비활성화
        OutputDebugStringA("Object deactivated safely\n");
    }
    
    // 🔥 FIXED: 실제 파괴는 Scene의 Update 끝에서 안전하게 처리
    // honmun->Destroy(); // 제거: 즉시 파괴 금지
}

// Aron_Scene.cpp에 안전한 정리 시스템 추가
void Aron_Scene::SafeCleanupDestroyedObjects()
{
    std::vector<Honmun*> toDestroy;
    
    // 1. 파괴 표시된 객체들 수집
    for (auto it = waveData.spawnedHonmuns.begin(); it != waveData.spawnedHonmuns.end();)
    {
        auto* honmun = *it;
        if (!honmun || !honmun->IsActive())
        {
            if (honmun) toDestroy.push_back(honmun);
            it = waveData.spawnedHonmuns.erase(it);
            OutputDebugStringA("Removed from spawnedHonmuns\n");
        }
        else
        {
            ++it;
        }
    }
    
    // 2. allHonmuns에서도 제거
    for (auto it = allHonmuns.begin(); it != allHonmuns.end();)
    {
        auto* honmun = *it;
        if (!honmun || !honmun->IsActive())
        {
            it = allHonmuns.erase(it);
            OutputDebugStringA("Removed from allHonmuns\n");
        }
        else
        {
            ++it;
        }
    }
    
    // 3. honmunManager에서도 제거
    for (auto it = honmunManager.activeHonmuns.begin(); it != honmunManager.activeHonmuns.end();)
    {
        auto* honmun = *it;
        if (!honmun || !honmun->IsActive())
        {
            it = honmunManager.activeHonmuns.erase(it);
            OutputDebugStringA("Removed from activeHonmuns\n");
        }
        else
        {
            ++it;
        }
    }
    
    // 4. 마지막에 안전하게 파괴
    for (auto* honmun : toDestroy)
    {
        if (honmun)
        {
            char debugMsg[100];
            sprintf_s(debugMsg, "SAFELY destroying %s\n", honmun->name.c_str());
            OutputDebugStringA(debugMsg);
            
            honmun->Destroy();  // 이제 안전하게 파괴
        }
    }
    
    if (!toDestroy.empty())
    {
        char debugMsg[100];
        sprintf_s(debugMsg, "SafeCleanup: Destroyed %zu objects safely\n", toDestroy.size());
        OutputDebugStringA(debugMsg);
    }
}

// Aron_Scene::Update() 끝에 추가
void Aron_Scene::Update()
{
    __super::Update();
    
    // ... 기존 업데이트 로직 ...
    
    // 🔥 NEW: 프레임 끝에 안전한 정리
    SafeCleanupDestroyedObjects();
}

// 안전한 벡터 순회를 위한 헬퍼 함수
bool Aron_Scene::IsValidHonmun(Honmun* honmun)
{
    return honmun != nullptr && honmun->IsActive();
}

// 모든 벡터 순회 시 안전성 체크 추가
void Aron_Scene::UpdateHonmunManager()
{
    // 🔥 FIXED: 안전한 순회
    std::vector<Honmun*> validHonmuns;
    
    for (auto* honmun : honmunManager.activeHonmuns)
    {
        if (IsValidHonmun(honmun))
        {
            validHonmuns.push_back(honmun);
        }
    }
    
    honmunManager.activeHonmuns = validHonmuns;
    honmunManager.currentCount = static_cast<int>(validHonmuns.size());
}

// UpdateWaveSystem도 안전하게 수정
void Aron_Scene::UpdateWaveSystem()
{
    if (!waveData.waveActive) return;
    
    // ... 기존 로직 ...
    
    // 🔥 FIXED: 안전한 적 체크
    for (auto it = waveData.spawnedHonmuns.begin(); it != waveData.spawnedHonmuns.end();)
    {
        auto* honmun = *it;
        if (!IsValidHonmun(honmun))
        {
            it = waveData.spawnedHonmuns.erase(it);  // 즉시 제거
        }
        else
        {
            if (IsHonmunOnGround(honmun))
            {
                waveData.spawnInterval = 0.5f; // 더 빨리 스폰
            }
            ++it;
        }
    }
}