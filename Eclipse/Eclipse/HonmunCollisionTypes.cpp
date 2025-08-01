#include "HonmunCollisionTypes.h"
#include "HonmunCollisionEffects.h"
#include "HonmunCollisionManager.h"
#include "Aron_Scene.h"
#include "../Direct2D_EngineLib/SceneManager.h"

void HonmunCollisionTypes::ProcessCollision(HonmunCollisionBase* script1, HonmunCollisionBase* script2, const ContactInfo& contact)
{
    if (!ShouldProcessCollision(script1, script2)) return;

    HonmunType type1 = script1->GetHonmunType();
    HonmunType type2 = script2->GetHonmunType();

    // Same type collisions
    if (type1 == type2)
    {
        switch (type1)
        {
        case HonmunType::A:
            HandleIgnisReaction(script1, script2);
            break;
        case HonmunType::B:
            HandleUmbraReaction(script1, script2);
            break;
        case HonmunType::C:
            HandleDarknessReaction(script1, script2);
            break;
        case HonmunType::D:
            HandleLunaReaction(script1, script2);
            break;
        }
    }
    else
    {
        // Mixed type collision
        HandleMixedReaction(script1, script2);
    }
}

void HonmunCollisionTypes::HandleIgnisReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // A + A = 체력 감소 + 합체 시스템 (체력 2 이상 남으면 합체)
    
    // 점수 시스템을 위한 현재 씬 가져오기
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    // 체력 감소
    int health1 = script->GetHealth() - 1;
    int health2 = otherScript->GetHealth() - 1;
    
    script->SetHealth(health1);
    otherScript->SetHealth(health2);
    
    // 체력이 0이 되면 완전 파괴 + 점수 부여
    if (health1 <= 0 || health2 <= 0)
    {
        OutputDebugStringA("A type health reached 0, destroying objects!\n");
        
        if (health1 <= 0) 
        {
            // 파괴되는 A가 2A인지 확인 (크기가 큰 경우)
            if (aronScene)
            {
                bool is2A = (script->GetCurrentSize() > 15.0f);
                int points = is2A ? 3 : 1;
                aronScene->AddScore(points);
                
                char debugMsg[100];
                sprintf_s(debugMsg, "A destroyed! Was 2A: %s, Points: %d\n", is2A ? "Yes" : "No", points);
                OutputDebugStringA(debugMsg);
            }
            script->DestroyThis();
        }
        
        if (health2 <= 0) 
        {
            // 파괴되는 A가 2A인지 확인
            if (aronScene)
            {
                bool is2A = (otherScript->GetCurrentSize() > 15.0f);
                int points = is2A ? 3 : 1;
                aronScene->AddScore(points);
                
                char debugMsg[100];
                sprintf_s(debugMsg, "A destroyed! Was 2A: %s, Points: %d\n", is2A ? "Yes" : "No", points);
                OutputDebugStringA(debugMsg);
            }
            otherScript->DestroyThis();
        }
    }
    else
    {
        // 둘 다 생존: 합체 시스템
        OutputDebugStringA("A + A collision, both survived - merging!\n");
        
        if (script->GetCollisionEffects())
        {
            script->GetCollisionEffects()->MergeWithOther(script, otherScript);
        }
        
        if (aronScene)
        {
            aronScene->AddScore(2); // 합체 보너스 점수
        }
    }
}

void HonmunCollisionTypes::HandleUmbraReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // B + B = 체력 감소 + 분열 시스템
    
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    // 체력 감소
    int health1 = script->GetHealth() - 1;
    int health2 = otherScript->GetHealth() - 1;
    
    script->SetHealth(health1);
    otherScript->SetHealth(health2);
    
    // 체력이 0이 되면 파괴 + 분열
    if (health1 <= 0 || health2 <= 0)
    {
        OutputDebugStringA("B type health reached 0, destroying and splitting!\n");
        
        if (health1 <= 0)
        {
            if (aronScene) aronScene->AddScore(1);
            
            // 분열 생성
            if (script->GetCollisionEffects())
            {
                script->GetCollisionEffects()->CreateSplitObjectsWithCollision(script, 2, otherScript);
            }
            script->DestroyThis();
        }
        
        if (health2 <= 0)
        {
            if (aronScene) aronScene->AddScore(1);
            
            // 분열 생성
            if (otherScript->GetCollisionEffects())
            {
                otherScript->GetCollisionEffects()->CreateSplitObjectsWithCollision(otherScript, 2, script);
            }
            otherScript->DestroyThis();
        }
    }
    else
    {
        // 둘 다 생존: 서로 튕김
        OutputDebugStringA("B + B collision, both survived - bouncing!\n");
        
        if (script->GetCollisionEffects())
        {
            script->GetCollisionEffects()->BounceAwayKinematic(script, otherScript);
        }
        
        if (aronScene) aronScene->AddScore(1);
    }
}

void HonmunCollisionTypes::HandleDarknessReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // C + C = 체력 감소 + 파괴 시스템
    
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    OutputDebugStringA("C + C collision - mutual destruction!\n");
    
    // 둘 다 즉시 파괴
    if (aronScene)
    {
        aronScene->AddScore(2); // C 타입 파괴 점수
        aronScene->AddScore(2); // 상대방 C 타입 파괴 점수
    }
    
    script->DestroyThis();
    otherScript->DestroyThis();
}

void HonmunCollisionTypes::HandleLunaReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // D + D = 체력 감소 + 흡수 시스템
    
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    // 체력 감소
    int health1 = script->GetHealth() - 1;
    int health2 = otherScript->GetHealth() - 1;
    
    script->SetHealth(health1);
    otherScript->SetHealth(health2);
    
    if (health1 <= 0 || health2 <= 0)
    {
        OutputDebugStringA("D type health reached 0, destroying!\n");
        
        if (health1 <= 0)
        {
            if (aronScene) aronScene->AddScore(2);
            script->DestroyThis();
        }
        
        if (health2 <= 0)
        {
            if (aronScene) aronScene->AddScore(2);
            otherScript->DestroyThis();
        }
    }
    else
    {
        // 둘 다 생존: 주변 적 흡수
        OutputDebugStringA("D + D collision, both survived - absorbing nearby!\n");
        
        if (script->GetCollisionEffects())
        {
            Vector2 collisionPoint = script->GetTransform()->GetPosition();
            script->GetCollisionEffects()->AbsorbNearbyEnemies(script, collisionPoint);
        }
        
        if (aronScene) aronScene->AddScore(3);
    }
}

void HonmunCollisionTypes::HandleMixedReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // 서로 다른 타입 간의 충돌: 서로 밀어내기
    OutputDebugStringA("Mixed type collision - pushing away!\n");
    
    if (script->GetCollisionEffects())
    {
        script->GetCollisionEffects()->PushSideways(script, otherScript);
    }
    
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    if (aronScene)
    {
        aronScene->AddScore(1); // 혼합 충돌 점수
    }
}

bool HonmunCollisionTypes::ShouldProcessCollision(HonmunCollisionBase* script1, HonmunCollisionBase* script2)
{
    // 이미 처리 중이거나 null인 경우 처리하지 않음
    return script1 && script2 && 
           !script1->IsProcessingReaction() && 
           !script2->IsProcessingReaction();
}

void HonmunCollisionTypes::ApplyHealthChange(HonmunCollisionBase* script, int healthChange)
{
    int newHealth = script->GetHealth() + healthChange;
    script->SetHealth(newHealth);
}