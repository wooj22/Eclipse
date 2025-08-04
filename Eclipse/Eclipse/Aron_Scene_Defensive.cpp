#include "Aron_Scene.h"
#include "EclipseApp.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/Rigidbody.h"
#include "../Direct2D_EngineLib/CircleCollider.h"
#include "../Direct2D_EngineLib/Vector2.h"
#include "Player.h"
#include "PlayerFSM.h"
#include "Attack_State.h"
#include "Honmun.h"
#include <vector>
#include <memory>
#include <mutex>
#include <random>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <Windows.h>

// 방어적 프로그래밍으로 try-catch 지옥 제거

// 기존의 위험한 마우스 클릭 처리
/*
if (Input::GetMouseButtonDown(0)) {
    OutputDebugStringA("Mouse left click detected\n");
    
    if (player && player->IsActive()) {
        auto* playerFSM = player->GetComponent<PlayerFSM>();
        if (playerFSM) {
            try {
                auto* movementFSM = playerFSM->GetMovementFSM();
                if (movementFSM) {
                    movementFSM->ChangeState(std::make_unique<Attack_State>());
                    OutputDebugStringA("Mouse click - Player attack triggered!\n");
                } else {
                    OutputDebugStringA("MovementFSM is null\n");
                }
            } catch (...) {
                OutputDebugStringA("Exception in mouse click attack handling\n");
            }
        } else {
            OutputDebugStringA("PlayerFSM is null\n");
        }
    } else {
        OutputDebugStringA("Player is null or inactive\n");
    }
}
*/

// 개선된 안전한 마우스 클릭 처리
void Aron_Scene::HandleMouseClick()
{
    if (!Input::GetMouseButtonDown(0)) return;
    
    // RAII 가드 패턴
    class PlayerAttackGuard {
    private:
        Player* player_;
        bool validState_;
        
    public:
        PlayerAttackGuard(Player* p) : player_(p), validState_(false) {
            if (player_ && player_->IsActive()) {
                auto* fsm = player_->GetComponent<PlayerFSM>();
                if (fsm && fsm->GetMovementFSM()) {
                    validState_ = true;
                }
            }
        }
        
        bool IsValid() const { return validState_; }
        
        bool TriggerAttack() {
            if (!validState_) return false;
            
            auto* fsm = player_->GetComponent<PlayerFSM>();
            auto* movementFSM = fsm->GetMovementFSM();
            
            // 방어적 상태 체크
            if (movementFSM) {
                movementFSM->ChangeState(std::make_unique<Attack_State>());
                return true;
            }
            return false;
        }
    };
    
    PlayerAttackGuard guard(this->player);
    if (guard.IsValid()) {
        if (guard.TriggerAttack()) {
            OutputDebugStringA("Mouse click - Player attack triggered!\n");
        } else {
            OutputDebugStringA("Failed to trigger attack - invalid state\n");
        }
    } else {
        OutputDebugStringA("Player attack unavailable\n");
    }
}

// 기존의 위험한 웨이브 정리 시스템
/*
std::vector<Honmun*> allHonmunsCopy;
try {
    allHonmunsCopy = allHonmuns;
} catch (...) {
    OutputDebugStringA("ERROR: Failed to copy allHonmuns vector, skipping cleanup\n");
    allHonmuns.clear();
    selectedHonmunIndex = 0;
    return;
}

for (auto* honmun : allHonmunsCopy) {
    if (honmun) {
        try {
            if (honmun->IsActive()) {
                honmun->SetActive(false);
                OutputDebugStringA("Deactivated test Honmun\n");
            }
        } catch (...) {
            OutputDebugStringA("ERROR: Exception while deactivating Honmun\n");
        }
    }
}
*/

// 개선된 안전한 웨이브 정리
void Aron_Scene::SafeCleanupWave()
{
    std::lock_guard<std::mutex> lock(honmunMutex);
    
    // 방어적 검증
    if (honmunRegistry.empty()) {
        OutputDebugStringA("No Honmuns to cleanup\n");
        return;
    }
    
    size_t cleanedCount = 0;
    std::vector<int> toRemove;
    
    // 안전한 반복자 사용
    for (const auto& pair : honmunRegistry) {
        if (!pair.second) {
            toRemove.push_back(pair.first);
            continue;
        }
        
        // 방어적 상태 체크
        if (pair.second->IsActive()) {
            pair.second->SetActive(false);
            cleanedCount++;
        }
        toRemove.push_back(pair.first);
    }
    
    // 안전한 제거
    for (int id : toRemove) {
        honmunRegistry.erase(id);
    }
    
    // 웨이브 데이터 정리
    waveData.spawnedHonmunIds.clear();
    waveData.waveActive = false;
    waveData.currentSpawnIndex = 0;
    
    selectedHonmunId = -1;
    honmunManager.currentCount = 0;
    
    char debugMsg[100];
    sprintf_s(debugMsg, "Safely cleaned %zu Honmuns\n", cleanedCount);
    OutputDebugStringA(debugMsg);
}

// 방어적 혼문 상태 검증 시스템
class HonmunValidator {
public:
    static bool IsValidForOperation(std::shared_ptr<Honmun> honmun) {
        if (!honmun) return false;
        if (!honmun->IsActive()) return false;
        
        auto* transform = honmun->GetComponent<Transform>();
        auto* rigidbody = honmun->GetComponent<Rigidbody>();
        auto* collider = honmun->GetComponent<CircleCollider>();
        
        return transform && rigidbody && collider;
    }
    
    static bool IsValidPosition(const Vector2& pos) {
        return !std::isnan(pos.x) && !std::isnan(pos.y) && 
               std::isfinite(pos.x) && std::isfinite(pos.y);
    }
    
    static bool IsInBounds(const Vector2& pos, float boundaryX = 5000.0f, float boundaryY = 5000.0f) {
        return std::abs(pos.x) <= boundaryX && std::abs(pos.y) <= boundaryY;
    }
};

// 개선된 안전한 업데이트 루프
void Aron_Scene::SafeUpdateHonmuns()
{
    std::lock_guard<std::mutex> lock(this->honmunMutex);
    
    std::vector<int> invalidHonmuns;
    
    for (const auto& pair : this->honmunRegistry) {
        auto honmun = pair.second;
        
        // 방어적 검증
        if (!HonmunValidator::IsValidForOperation(honmun)) {
            invalidHonmuns.push_back(pair.first);
            continue;
        }
        
        // 안전한 위치 검증
        auto* transform = honmun->GetComponent<Transform>();
        if (!transform) {
            invalidHonmuns.push_back(pair.first);
            continue;
        }
        Vector2 pos = transform->GetPosition();
        
        if (!HonmunValidator::IsValidPosition(pos)) {
            char debugMsg[100];
            sprintf_s(debugMsg, "Invalid position detected for Honmun ID %d: (%.2f, %.2f)\n", 
                pair.first, pos.x, pos.y);
            OutputDebugStringA(debugMsg);
            
            // 안전한 위치로 리셋
            transform->SetPosition(0.0f, 0.0f);
            continue;
        }
        
        if (!HonmunValidator::IsInBounds(pos)) {
            invalidHonmuns.push_back(pair.first);
        }
    }
    
    // 안전한 정리
    for (int id : invalidHonmuns) {
        this->RemoveHonmun(id);
    }
    
    if (!invalidHonmuns.empty()) {
        char debugMsg[100];
        sprintf_s(debugMsg, "Removed %zu invalid Honmuns\n", invalidHonmuns.size());
        OutputDebugStringA(debugMsg);
    }
}