// 스레드 안전한 혼문 충돌 관리 시스템

#include "HonmunCollisionManager.h"
#include "HonmunCollisionBase.h"
#include "../Direct2D_EngineLib/Vector2.h"
#include "../Direct2D_EngineLib/Transform.h"
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <vector>
#include <algorithm>
#include <memory>
#include <chrono>
#include <thread>
#include <cmath>
#include <cstdio>
#include <Windows.h>

class ThreadSafeHonmunCollisionManager {
private:
    mutable std::shared_mutex registryMutex;
    std::vector<HonmunCollisionBase*> registeredHonmuns;
    std::atomic<bool> isUpdating{false};
    
    // 읽기-쓰기 락 패턴
    class ReadLock {
        std::shared_lock<std::shared_mutex> lock;
    public:
        ReadLock(std::shared_mutex& mutex) : lock(mutex) {}
    };
    
    class WriteLock {
        std::unique_lock<std::shared_mutex> lock;
    public:
        WriteLock(std::shared_mutex& mutex) : lock(mutex) {}
    };

public:
    // 안전한 혼문 등록
    void RegisterHonmun(HonmunCollisionBase* script) {
        if (!script) return;
        
        WriteLock lock(registryMutex);
        
        // 중복 등록 방지
        auto it = std::find(registeredHonmuns.begin(), registeredHonmuns.end(), script);
        if (it == registeredHonmuns.end()) {
            registeredHonmuns.push_back(script);
            
            char debugMsg[100];
            sprintf_s(debugMsg, "Thread-safe registration: Total count: %zu\n", registeredHonmuns.size());
            OutputDebugStringA(debugMsg);
        }
    }
    
    // 안전한 혼문 등록 해제
    void UnregisterHonmun(HonmunCollisionBase* script) {
        if (!script) return;
        
        WriteLock lock(registryMutex);
        
        auto originalSize = registeredHonmuns.size();
        registeredHonmuns.erase(
            std::remove(registeredHonmuns.begin(), registeredHonmuns.end(), script),
            registeredHonmuns.end()
        );
        
        auto newSize = registeredHonmuns.size();
        if (newSize < originalSize) {
            char debugMsg[100];
            sprintf_s(debugMsg, "Thread-safe unregistration: Remaining count: %zu\n", newSize);
            OutputDebugStringA(debugMsg);
        }
    }
    
    // 안전한 근처 혼문 검색
    std::vector<HonmunCollisionBase*> GetNearbyHonmuns(HonmunCollisionBase* script, float radius) {
        std::vector<HonmunCollisionBase*> nearbyHonmuns;
        
        if (!script || !script->GetTransform()) {
            return nearbyHonmuns;
        }
        
        ReadLock lock(registryMutex);
        
        Vector2 centerPos = script->GetTransform()->GetPosition();
        
        // 읽기 전용 복사본 생성 (락 하에서)
        std::vector<HonmunCollisionBase*> registeredCopy = registeredHonmuns;
        
        // 락 해제 후 계산 (데드락 방지)
        lock.~ReadLock();
        
        for (auto* honmun : registeredCopy) {
            if (!IsValidHonmun(honmun) || honmun == script) continue;
            
            if (!honmun->GetTransform()) continue;
            
            Vector2 honmunPos = honmun->GetTransform()->GetPosition();
            float distance = CalculateDistance(centerPos, honmunPos);
            
            if (distance <= radius) {
                nearbyHonmuns.push_back(honmun);
            }
        }
        
        return nearbyHonmuns;
    }
    
    // 안전한 정리 작업
    void CleanupDestroyedObjects() {
        // 원자적 플래그로 동시 실행 방지
        bool expected = false;
        if (!isUpdating.compare_exchange_strong(expected, true)) {
            OutputDebugStringA("Cleanup already in progress, skipping\n");
            return;
        }
        
        // RAII 가드로 플래그 자동 해제
        class CleanupGuard {
            std::atomic<bool>& flag;
        public:
            CleanupGuard(std::atomic<bool>& f) : flag(f) {}
            ~CleanupGuard() { flag.store(false); }
        } guard(isUpdating);
        
        WriteLock lock(registryMutex);
        
        auto originalSize = registeredHonmuns.size();
        
        registeredHonmuns.erase(
            std::remove_if(registeredHonmuns.begin(), registeredHonmuns.end(),
                [this](HonmunCollisionBase* script) { return !IsValidHonmun(script); }),
            registeredHonmuns.end()
        );
        
        auto newSize = registeredHonmuns.size();
        if (newSize < originalSize) {
            char debugMsg[100];
            sprintf_s(debugMsg, "Thread-safe cleanup: Removed %zu invalid objects, remaining: %zu\n", 
                     originalSize - newSize, newSize);
            OutputDebugStringA(debugMsg);
        }
    }
    
private:
    bool IsValidHonmun(HonmunCollisionBase* script) {
        return script != nullptr && 
               script->GetTransform() != nullptr && 
               !script->IsMarkedForDestroy();
    }
    
    float CalculateDistance(const Vector2& pos1, const Vector2& pos2) {
        float dx = pos1.x - pos2.x;
        float dy = pos1.y - pos2.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

// 스레드 안전한 Aron_Scene 업데이트 시스템
class ThreadSafeAronScene {
private:
    mutable std::shared_mutex sceneMutex;
    std::atomic<bool> updateInProgress{false};
    std::atomic<bool> sceneExiting{false};
    
public:
    // 안전한 업데이트 루프
    void SafeUpdate() {
        // 씬 종료 중이면 업데이트 스킵
        if (sceneExiting.load()) return;
        
        // 원자적 업데이트 플래그
        bool expected = false;
        if (!updateInProgress.compare_exchange_strong(expected, true)) {
            return; // 다른 스레드가 이미 업데이트 중
        }
        
        // RAII 가드로 플래그 자동 해제
        class UpdateGuard {
            std::atomic<bool>& flag;
        public:
            UpdateGuard(std::atomic<bool>& f) : flag(f) {}
            ~UpdateGuard() { flag.store(false); }
        } guard(updateInProgress);
        
        try {
            std::shared_lock<std::shared_mutex> lock(sceneMutex);
            
            // 안전한 업데이트 로직
            ProcessSafeInput();
            UpdateSafeWaveSystem();
            UpdateSafeHonmunManager();
            UpdateSafeUI();
            
        } catch (const std::exception& e) {
            char debugMsg[200];
            sprintf_s(debugMsg, "Exception in SafeUpdate: %s\n", e.what());
            OutputDebugStringA(debugMsg);
        }
    }
    
    // 안전한 씬 종료
    void SafeExit() {
        sceneExiting.store(true);
        
        // 업데이트 완료 대기
        while (updateInProgress.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        
        std::unique_lock<std::shared_mutex> lock(sceneMutex);
        
        // 안전한 정리 작업
        CleanupAllResources();
        
        OutputDebugStringA("Scene safely exited\n");
    }
    
private:
    void ProcessSafeInput() {
        // 입력 처리 로직 (이미 단일 스레드에서 실행됨)
    }
    
    void UpdateSafeWaveSystem() {
        // 웨이브 시스템 업데이트 (원자적 연산 사용)
    }
    
    void UpdateSafeHonmunManager() {
        // 혼문 관리자 업데이트 (스레드 안전)
    }
    
    void UpdateSafeUI() {
        // UI 업데이트 (메인 스레드에서만)
    }
    
    void CleanupAllResources() {
        // 모든 리소스 정리
    }
};

// 개선된 웨이브 스폰 시스템 (원자적 연산)
class AtomicWaveSpawner {
private:
    std::atomic<int> spawnedCount{0};
    std::atomic<bool> spawnInProgress{false};
    mutable std::mutex spawnMutex;
    
public:
    bool TrySpawn(int targetCount) {
        if (spawnedCount.load() >= targetCount) return false;
        
        bool expected = false;
        if (!spawnInProgress.compare_exchange_strong(expected, true)) {
            return false; // 다른 스레드가 이미 스폰 중
        }
        
        std::lock_guard<std::mutex> lock(spawnMutex);
        
        // 더블 체크 패턴
        if (spawnedCount.load() >= targetCount) {
            spawnInProgress.store(false);
            return false;
        }
        
        // 실제 스폰 로직
        bool success = PerformSpawn();
        
        if (success) {
            spawnedCount.fetch_add(1);
        }
        
        spawnInProgress.store(false);
        return success;
    }
    
    void Reset() {
        std::lock_guard<std::mutex> lock(spawnMutex);
        spawnedCount.store(0);
        spawnInProgress.store(false);
    }
    
    int GetSpawnedCount() const {
        return spawnedCount.load();
    }
    
private:
    bool PerformSpawn() {
        // 실제 혼문 생성 로직
        return true;
    }
};

// 싱글톤 스레드 안전 관리자
class ThreadSafeGameManager {
private:
    static std::mutex instanceMutex;
    static std::unique_ptr<ThreadSafeGameManager> instance;
    
    ThreadSafeGameManager() = default;
    
public:
    static ThreadSafeGameManager& Get() {
        std::lock_guard<std::mutex> lock(instanceMutex);
        if (!instance) {
            instance.reset(new ThreadSafeGameManager());
        }
        return *instance;
    }
    
    // 스레드 안전한 상태 업데이트
    void SafeUpdateWaveCount(int count) {
        std::lock_guard<std::mutex> lock(instanceMutex);
        // 원자적 업데이트 로직
    }
};

std::mutex ThreadSafeGameManager::instanceMutex;
std::unique_ptr<ThreadSafeGameManager> ThreadSafeGameManager::instance;