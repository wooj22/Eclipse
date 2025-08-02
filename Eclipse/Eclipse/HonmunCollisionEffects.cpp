#include "HonmunCollisionEffects.h"
#include "HonmunCollisionManager.h"
#include "Aron_Scene.h"  // 혼문 관리 시스템 연동
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
    // TODO: Rigidbody velocity API 구현 후 활성화
    /*
    auto* rigidbody = script->GetRigidbody();
    if (rigidbody)
    {
        Vector2 currentVelocity = rigidbody->GetVelocity();
        rigidbody->SetVelocity(currentVelocity * speedMultiplier);
    }
    */
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

void HonmunCollisionEffects::CreateSplitFragments(HonmunCollisionBase* script, const Vector2& collisionPoint, int count, float newSize, float speedMultiplier)
{
    // B+B 분열: 4개 조각 생성
    char startMsg[100];
    sprintf_s(startMsg, "=== Creating %d split fragments ===\n", count);
    OutputDebugStringA(startMsg);
    
    for (int i = 0; i < count; i++)
    {
        // 방사형으로 퍼지는 방향 계산
        float angle = (2.0f * 3.14159f * i) / count; // 360도를 count로 나눔
        Vector2 direction(cos(angle), sin(angle));
        Vector2 spawnPos = collisionPoint + direction * 60.0f; // 충돌 지점에서 60픽셀 떨어진 곳에 생성
        
        // Scene::CreateObject 방식으로 변경하여 올바른 타입으로 생성
        HonmunType originalType = script->GetHonmunType();
        char typeDebugMsg[100];
        sprintf_s(typeDebugMsg, "Creating fragment with original type: %d\n", static_cast<int>(originalType));
        OutputDebugStringA(typeDebugMsg);
        
        // Scene에서 직접 생성하는 방법 시도
        auto* currentScene = SceneManager::Get().GetCurrentScene();
        auto* fragmentHonmun = currentScene->CreateObject<Honmun>(spawnPos);
        
        if (!fragmentHonmun) {
            char errorMsg[100];
            sprintf_s(errorMsg, "ERROR: Failed to create fragment %d\n", i+1);
            OutputDebugStringA(errorMsg);
            continue;
        }
        
        // B 분열 조각은 소문자 'b' 타입으로 설정
        HonmunType fragmentType = (originalType == HonmunType::B) ? HonmunType::b : originalType;
        
        // 이름 설정 (플레이어 공격 감지용)
        char fragmentName[50];
        sprintf_s(fragmentName, "Honmun_b_Fragment_%d", i+1);
        fragmentHonmun->name = fragmentName;
        
        fragmentHonmun->SetHonmunType(fragmentType);
        
        char successMsg[100];
        sprintf_s(successMsg, "SUCCESS: Fragment %d created and type set to %d (b-type)\n", i+1, static_cast<int>(fragmentType));
        OutputDebugStringA(successMsg);
        
        auto* fragmentScript = fragmentHonmun->GetComponent<HonmunCollisionBase>();
        if (!fragmentScript) {
            fragmentScript = fragmentHonmun->AddComponent<HonmunCollisionBase>();
        }
        fragmentScript->SetCurrentSize(newSize);
        fragmentScript->SetHealth(1); // b 조각들은 HP 1
        fragmentScript->SetSplitFragment(true);
        fragmentScript->SetHonmunType(fragmentType); // 스크립트에도 올바른 타입 설정
        
        // 크기 적용
        if (fragmentScript->GetHonmun())
        {
            fragmentScript->GetHonmun()->SetSize(newSize);
        }
        
        // 충돌 테스트용: 키네마틱 모드 유지
        auto* rigidbody = fragmentScript->GetRigidbody();
        if (rigidbody)
        {
            rigidbody->useGravity = false; // 충돌 테스트용으로 중력 비활성화
            rigidbody->isKinematic = true;  // 키네마틱 모드 유지
            rigidbody->velocity = Vector2(0, 0); // 속도 초기화
        }
        
        // 혼문 관리 시스템에 추가
        auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
        if (aronScene) {
            aronScene->AddHonmunToManager(fragmentHonmun);
        }
        
        char debugMsg[150];
        sprintf_s(debugMsg, "Fragment %d: pos(%.1f,%.1f), size %.2f, speed x%.1f, added to manager\n", 
                 i+1, spawnPos.x, spawnPos.y, newSize, speedMultiplier);
        OutputDebugStringA(debugMsg);
    }
    
    char finalMsg[100];
    sprintf_s(finalMsg, "=== TOTAL %d fragments created successfully ===\n", count);
    OutputDebugStringA(finalMsg);
}

void HonmunCollisionEffects::AttractAndDestroyEnemies(HonmunCollisionBase* script, const Vector2& attractionPoint, float pullRatio)
{
    // C+C 효과: 카메라 시야 내 적들을 끌어당기기
    if (!script->GetCollisionManager()) return;
    
    auto nearbyHonmuns = script->GetCollisionManager()->GetNearbyHonmuns(script, 500.0f); // 큰 반경으로 검색
    
    int attractedCount = 0;
    for (auto* nearbyScript : nearbyHonmuns)
    {
        if (nearbyScript != script)
        {
            Vector2 currentPos = nearbyScript->GetTransform()->GetPosition();
            Vector2 toAttraction = attractionPoint - currentPos;
            Vector2 newPos = currentPos + toAttraction * pullRatio; // 1/3 거리만큼 당김
            
            nearbyScript->GetTransform()->SetPosition(newPos.x, newPos.y);
            attractedCount++;
        }
    }
    
    char debugMsg[100];
    sprintf_s(debugMsg, "C+C attraction: %d enemies pulled by %.1f%% distance\n", 
             attractedCount, pullRatio * 100.0f);
    OutputDebugStringA(debugMsg);
}

void HonmunCollisionEffects::BounceOppositeDirections(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // A+B 효과: 반대 방향으로 튕김
    Vector2 pos1 = script->GetTransform()->GetPosition();
    Vector2 pos2 = otherScript->GetTransform()->GetPosition();
    
    Vector2 direction = (pos1 - pos2).Normalized();
    float bounceDistance = 80.0f;
    
    // 서로 반대 방향으로 밀어냄
    script->GetTransform()->SetPosition(pos1 + direction * bounceDistance);
    otherScript->GetTransform()->SetPosition(pos2 - direction * bounceDistance);
    
    // 속도도 부여
    auto* rb1 = script->GetRigidbody();
    auto* rb2 = otherScript->GetRigidbody();
    if (rb1 && rb2)
    {
        rb1->velocity = Vector2(direction.x * 150.0f, rb1->velocity.y);
        rb2->velocity = Vector2(-direction.x * 150.0f, rb2->velocity.y);
    }
    
    OutputDebugStringA("A+B bounce: Objects bounced in opposite directions!\n");
}

void HonmunCollisionEffects::PushLeftRight(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // A+C 효과: 좌우로 밀어냄
    Vector2 pos1 = script->GetTransform()->GetPosition();
    Vector2 pos2 = otherScript->GetTransform()->GetPosition();
    
    // 좌우 방향 결정
    float pushDistance = 100.0f;
    Vector2 leftDirection(-1.0f, 0.0f);
    Vector2 rightDirection(1.0f, 0.0f);
    
    // 첫 번째는 왼쪽, 두 번째는 오른쪽으로
    script->GetTransform()->SetPosition(pos1 + leftDirection * pushDistance);
    otherScript->GetTransform()->SetPosition(pos2 + rightDirection * pushDistance);
    
    // 좌우 속도 부여
    auto* rb1 = script->GetRigidbody();
    auto* rb2 = otherScript->GetRigidbody();
    if (rb1 && rb2)
    {
        rb1->velocity = Vector2(-120.0f, rb1->velocity.y);
        rb2->velocity = Vector2(120.0f, rb2->velocity.y);
    }
    
    OutputDebugStringA("A+C push: Objects pushed left and right!\n");
}

void HonmunCollisionEffects::PenetrateWithoutOverlap(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // B+C 효과: 겹침 없이 관통
    Vector2 pos1 = script->GetTransform()->GetPosition();
    Vector2 pos2 = otherScript->GetTransform()->GetPosition();
    
    Vector2 direction = (pos1 - pos2).Normalized();
    float separationDistance = 70.0f; // 겹치지 않도록 최소 거리 확보
    
    // 서로 약간 떨어뜨림
    script->GetTransform()->SetPosition(pos1 + direction * separationDistance * 0.5f);
    otherScript->GetTransform()->SetPosition(pos2 - direction * separationDistance * 0.5f);
    
    // 관통 효과: 서로의 속도를 유지하며 지나감
    auto* rb1 = script->GetRigidbody();
    auto* rb2 = otherScript->GetRigidbody();
    if (rb1 && rb2)
    {
        // 현재 속도 유지하면서 약간의 관통 효과
        Vector2 vel1 = rb1->velocity;
        Vector2 vel2 = rb2->velocity;
        rb1->velocity = Vector2(vel1.x + direction.x * 30.0f, vel1.y);
        rb2->velocity = Vector2(vel2.x - direction.x * 30.0f, vel2.y);
    }
    
    OutputDebugStringA("B+C penetrate: Objects penetrated without overlap!\n");
}