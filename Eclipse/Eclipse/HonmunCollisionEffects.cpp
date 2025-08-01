#include "HonmunCollisionEffects.h"
#include "HonmunCollisionManager.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/ResourceManager.h"
#include <random>

void HonmunCollisionEffects::MergeWithOther(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // 합체: 크기 증가 및 상대방 제거
    float newSize = script->GetCurrentSize() * 1.4f; // 40% 크기 증가
    UpdateSize(script, newSize);
    
    // 체력 증가
    int newHealth = script->GetHealth() + 1;
    script->SetHealth(newHealth);
    
    OutputDebugStringA("Merge completed - size and health increased!\n");
    
    // 상대방 제거
    otherScript->DestroyThis();
}

void HonmunCollisionEffects::SplitIntoTwo(HonmunCollisionBase* script)
{
    // 크기 30% 감소
    UpdateSize(script, script->GetCurrentSize() * 0.7f);
    
    // 낙하속도 20% 증가
    UpdateFallingSpeed(script, 1.2f);
    
    // 새로운 혼문 생성
    Vector2 currentPos = script->GetTransform()->GetPosition();
    Vector2 offset = GetRandomDirection() * 2.0f;
    
    // Script에서 Instantiate 사용해 새 혼문 생성
    auto* newHonmun = script->Instantiate<Honmun>(currentPos + offset);
    auto* newScript = newHonmun->AddComponent<HonmunCollisionBase>();
    newScript->SetHonmunType(script->GetHonmunType());
    newScript->SetCurrentSize(script->GetCurrentSize());
    newScript->SetHealth(1); // 에너지 1로 설정
    newScript->SetSplitFragment(true);
    
    ApplyPhysicsTransition(newScript);
}

void HonmunCollisionEffects::CreateSplitObjects(HonmunCollisionBase* script, int count)
{
    Vector2 currentPos = script->GetTransform()->GetPosition();
    
    for (int i = 0; i < count; i++)
    {
        Vector2 randomDirection = GetRandomDirection();
        Vector2 spawnPos = currentPos + randomDirection * (3.0f + i * 2.0f);
        
        auto* newHonmun = script->Instantiate<Honmun>(spawnPos);
        auto* newScript = newHonmun->AddComponent<HonmunCollisionBase>();
        newScript->SetHonmunType(script->GetHonmunType());
        newScript->SetCurrentSize(script->GetCurrentSize() * 0.6f); // 조각은 더 작게
        newScript->SetHealth(1);
        newScript->SetSplitFragment(true);
        
        ApplyPhysicsTransition(newScript);
    }
    
    char debugMsg[100];
    sprintf_s(debugMsg, "Created %d split objects!\n", count);
    OutputDebugStringA(debugMsg);
}

void HonmunCollisionEffects::CreateSplitObjectsWithCollision(HonmunCollisionBase* script, int count, HonmunCollisionBase* otherScript)
{
    Vector2 myPos = script->GetTransform()->GetPosition();
    Vector2 otherPos = otherScript->GetTransform()->GetPosition();
    
    // 충돌 방향 계산
    Vector2 collisionDirection = (myPos - otherPos).Normalized();
    
    OutputDebugStringA("CreateSplitObjectsWithCollision started!\n");
    
    for (int i = 0; i < count; i++)
    {
        // 충돌 방향으로 퍼지도록 스폰 위치 계산
        Vector2 spreadDirection = collisionDirection * (1.0f + i * 0.5f);
        Vector2 randomOffset = GetRandomDirection() * 1.5f;
        Vector2 spawnPos = myPos + spreadDirection * 50.0f + randomOffset;
        
        auto* splitHonmun = script->Instantiate<Honmun>(spawnPos);
        auto* splitScript = splitHonmun->AddComponent<HonmunCollisionBase>();
        splitScript->SetHonmunType(script->GetHonmunType());
        splitScript->SetCurrentSize(script->GetCurrentSize() * 0.5f);
        splitScript->SetHealth(1);
        splitScript->SetSplitFragment(true);
        
        ApplyPhysicsTransition(splitScript);
        
        char debugMsg[100];
        sprintf_s(debugMsg, "Split object %d created at (%.2f, %.2f)\n", i + 1, spawnPos.x, spawnPos.y);
        OutputDebugStringA(debugMsg);
    }
}

void HonmunCollisionEffects::AbsorbNearbyEnemies(HonmunCollisionBase* script, const Vector2& collisionPoint)
{
    if (!script->GetCollisionManager()) return;
    
    // 반경 내 혼문들 찾아서 흡수
    auto nearbyHonmuns = script->GetCollisionManager()->GetNearbyHonmuns(script, 100.0f);
    
    int absorbedCount = 0;
    for (auto* nearbyScript : nearbyHonmuns)
    {
        if (nearbyScript != script && absorbedCount < 3) // 최대 3개까지 흡수
        {
            // 크기 약간 증가
            float newSize = script->GetCurrentSize() * 1.1f;
            UpdateSize(script, newSize);
            
            // 흡수된 혼문 제거
            nearbyScript->DestroyThis();
            absorbedCount++;
        }
    }
    
    if (absorbedCount > 0)
    {
        char debugMsg[100];
        sprintf_s(debugMsg, "Absorbed %d nearby enemies!\n", absorbedCount);
        OutputDebugStringA(debugMsg);
    }
}

void HonmunCollisionEffects::BounceAway(HonmunCollisionBase* script, HonmunCollisionBase* otherScript, const ContactInfo& contact)
{
    // 물리 기반 튕김 (Rigidbody 사용)
    auto* rb1 = script->GetRigidbody();
    auto* rb2 = otherScript->GetRigidbody();
    
    if (rb1 && rb2)
    {
        Vector2 direction = (script->GetTransform()->GetPosition() - otherScript->GetTransform()->GetPosition()).Normalized();
        float bounceForce = 200.0f;
        
        rb1->AddForce(direction * bounceForce);
        rb2->AddForce(direction * -bounceForce);
    }
}

void HonmunCollisionEffects::BounceAwayKinematic(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // 키네마틱 모드에서의 튕김 (Transform 직접 조작)
    Vector2 pos1 = script->GetTransform()->GetPosition();
    Vector2 pos2 = otherScript->GetTransform()->GetPosition();
    
    Vector2 direction = (pos1 - pos2).Normalized();
    float pushDistance = 30.0f;
    
    script->GetTransform()->SetPosition(pos1 + direction * pushDistance);
    otherScript->GetTransform()->SetPosition(pos2 - direction * pushDistance);
    
    OutputDebugStringA("Kinematic bounce applied!\n");
}

void HonmunCollisionEffects::PushSideways(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // 옆으로 밀어내기
    Vector2 pos1 = script->GetTransform()->GetPosition();
    Vector2 pos2 = otherScript->GetTransform()->GetPosition();
    
    Vector2 direction = (pos1 - pos2).Normalized();
    float pushDistance = 40.0f;
    
    script->GetTransform()->SetPosition(pos1 + direction * pushDistance);
    otherScript->GetTransform()->SetPosition(pos2 - direction * pushDistance);
    
    OutputDebugStringA("Sideways push applied!\n");
}

void HonmunCollisionEffects::PassThrough(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // 통과: 특별한 물리 효과 없음
    OutputDebugStringA("Pass through - no physical effect!\n");
}

Vector2 HonmunCollisionEffects::GetRandomDirection()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    
    Vector2 direction(dis(gen), dis(gen));
    return direction.Normalized();
}

void HonmunCollisionEffects::UpdateSize(HonmunCollisionBase* script, float newSize)
{
    script->SetCurrentSize(newSize);
    
    if (script->GetHonmun())
    {
        script->GetHonmun()->SetSize(newSize / 10.0f); // 크기 정규화
    }
}

void HonmunCollisionEffects::UpdateFallingSpeed(HonmunCollisionBase* script, float speedMultiplier)
{
    auto* rigidbody = script->GetRigidbody();
    if (rigidbody)
    {
        Vector2 currentVelocity = rigidbody->GetVelocity();
        rigidbody->SetVelocity(currentVelocity * speedMultiplier);
    }
}

void HonmunCollisionEffects::ApplyPhysicsTransition(HonmunCollisionBase* script)
{
    // 분열된 조각에 물리 전환 설정
    auto* rigidbody = script->GetRigidbody();
    if (rigidbody)
    {
        rigidbody->useGravity = false;  // 일시적으로 중력 비활성화
        rigidbody->isKinematic = true;  // 일시적으로 키네마틱 모드
        
        // 0.1초 후 물리 활성화는 Base 클래스의 Update에서 처리
    }
}