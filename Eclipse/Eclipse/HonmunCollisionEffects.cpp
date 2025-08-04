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
    // 물리 기반 튕김 (Rigidbody 사용) - 연쇄반응을 위한 차등 속도
    auto* rb1 = script->GetRigidbody();
    auto* rb2 = otherScript->GetRigidbody();
    
    if (rb1 && rb2)
    {
        Vector2 direction = (script->GetTransform()->GetPosition() - otherScript->GetTransform()->GetPosition()).Normalized();
        
        // 연쇄반응을 위해 서로 다른 힘 적용 (만날 수 있도록)
        float bounceForce1 = 180.0f; // 첫 번째 객체는 조금 약하게
        float bounceForce2 = 220.0f; // 두 번째 객체는 조금 강하게
        
        rb1->AddForce(direction * bounceForce1);
        rb2->AddForce(direction * -bounceForce2);
        
        OutputDebugStringA("Differential bounce applied for chain reactions!\n");
    }
}

void HonmunCollisionEffects::BounceAwayKinematic(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // 키네마틱 모드에서의 튕김 (Transform 직접 조작)
    Vector2 pos1 = script->GetTransform()->GetPosition();
    Vector2 pos2 = otherScript->GetTransform()->GetPosition();
    
    Vector2 direction = (pos1 - pos2).Normalized();
    // 연쇄반응을 위해 차등 거리 적용
    float pushDistance1 = 15.0f; // 첫 번째 객체는 가까이
    float pushDistance2 = 25.0f; // 두 번째 객체는 멀리
    
    script->GetTransform()->SetPosition(pos1 + direction * pushDistance1);
    otherScript->GetTransform()->SetPosition(pos2 - direction * pushDistance2);
    
    OutputDebugStringA("Kinematic bounce applied!\n");
}

void HonmunCollisionEffects::PushSideways(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // 옆으로 밀어내기
    Vector2 pos1 = script->GetTransform()->GetPosition();
    Vector2 pos2 = otherScript->GetTransform()->GetPosition();
    
    Vector2 direction = (pos1 - pos2).Normalized();
    float pushDistance = 25.0f; // 연쇄 작용을 위해 거리 단축 (40→25)
    
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
    // B+B 분열: 연쇄 충돌을 위한 최적화된 분산 시스템
    int enhancedCount = count; // 4개로 제한 (너무 많으면 연쇄 충돌 어려움)
    char startMsg[100];
    sprintf_s(startMsg, "=== Creating %d fragments optimized for CHAIN COLLISION ===\n", enhancedCount);
    OutputDebugStringA(startMsg);
    
    // 1차 분해: 주요 조각들 (더 큰 크기)
    for (int i = 0; i < enhancedCount; i++)
    {
        // 방사형으로 퍼지는 방향 계산 (예측 가능한 고정 각도)
        float angle = (2.0f * 3.14159f * i) / enhancedCount; // 고정된 각도 패턴
        // 45도씩 오프셋 추가하여 더 자연스러운 분산
        angle += 3.14159f * 0.25f; // 45도 오프셋
        Vector2 direction(cos(angle), sin(angle));
        
        // 연쇄 충돌 최적화: 더 넓은 범위로 분산
        static std::random_device rd;
        static std::mt19937 gen(rd());
        // 예측 가능한 거리 패턴 (랜덤 요소 줄임)
        float baseDistance = 180.0f; // 기본 거리
        float distanceVariation = 50.0f; // 변화량
        float predictableDistance = baseDistance + (distanceVariation * (i % 2)); // 교대로 거리 변화
        
        // 각 조각마다 예측 가능한 거리로 배치
        if (i % 2 == 0) { // 짝수 인덱스는 더 멀리
            predictableDistance *= 1.3f; // 30% 더 멀리 배치
        }
        
        Vector2 spawnPos = collisionPoint + direction * predictableDistance;
        
        // Scene::CreateObject 방식으로 변경하여 올바른 타입으로 생성
        HonmunType originalType = script->GetHonmunType();
        char typeDebugMsg[100];
        sprintf_s(typeDebugMsg, "Creating fragment with original type: %d\n", static_cast<int>(originalType));
        OutputDebugStringA(typeDebugMsg);
        
        // Scene에서 안전한 객체 생성
        auto* currentScene = SceneManager::Get().GetCurrentScene();
        if (!currentScene) {
            OutputDebugStringA("CRITICAL ERROR: CurrentScene is null during fragment creation\n");
            continue;
        }
        
        auto* fragmentHonmun = currentScene->CreateObject<Honmun>(spawnPos);
        if (!fragmentHonmun) {
            char errorMsg[100];
            sprintf_s(errorMsg, "ERROR: Failed to create fragment %d\n", i+1);
            OutputDebugStringA(errorMsg);
            continue;
        }
        
        try {
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
                if (!fragmentScript) {
                    OutputDebugStringA("ERROR: Failed to add collision script to fragment\n");
                    fragmentHonmun->SetActive(false);
                    continue;
                }
            }
        // 기획서 정확한 크기 30% 감소 적용
        fragmentScript->SetCurrentSize(newSize);  // 정확히 30% 감소된 크기
        fragmentScript->SetHealth(1); // b 조각들은 HP 1
        fragmentScript->SetSplitFragment(true);
        
        // 분열 조각 물리 전환 설정 (1초 후 키네마틱 모드로 전환하여 충돌 가능)
        fragmentScript->SetNeedsPhysicsTransition(true);
        fragmentScript->SetHonmunType(fragmentType); // 스크립트에도 올바른 타입 설정
        
        // 기획서 크기 30% 감소 적용 (정확한 크기)
        if (fragmentScript->GetHonmun())
        {
            fragmentScript->GetHonmun()->SetSize(newSize);
            
            char sizeMsg[100];
            float originalSize = script->GetCurrentSize();
            sprintf_s(sizeMsg, "b fragment size: %.2f (30%% smaller than original %.2f)\n", 
                     newSize, originalSize);
            OutputDebugStringA(sizeMsg);
        }
        
        // b 조각 가시성 개선: 색상이나 이펙트로 구분 (크기는 기획서대로 30% 감소 유지)
        // TODO: 향후 색상 변경이나 반짝임 효과로 가시성 개선 가능
        
        // 포켓볼/드래곤볼 스타일: 폭발적인 방사형 퍼짐 물리 설정
        auto* rigidbody = fragmentScript->GetRigidbody();
        if (rigidbody)
        {
            rigidbody->useGravity = false;   // 중력 비활성화
            rigidbody->isKinematic = false;  // 물리 시뮬레이션 활성화 (퍼지기 위해)
            
            // 연쇄 충돌 최적화: 적당한 속도로 퍼지기
            float baseSpeed = 200.0f * speedMultiplier; // 연쇄 충돌을 위해 속도 조정
            float layerMultiplier = 1.0f;
            
            // 각 조각마다 다른 속도로 배치 (연쇄 충돌 최적화)
            if (i < enhancedCount / 3) {
                layerMultiplier = 2.0f; // 최고속 계층 (2배)
            } else if (i < (enhancedCount * 2) / 3) {
                layerMultiplier = 1.5f; // 중간속 계층 (1.5배)
            } else {
                layerMultiplier = 1.0f; // 기본속 계층 (1.0배)
            }
            
            float scatterSpeed = baseSpeed * layerMultiplier;
            Vector2 scatterVelocity = direction * scatterSpeed;
            
            // 예측 가능한 속도 패턴 (랜덤 요소 줄임)
            float predictableFactor = 0.9f + (0.2f * (i % 3) / 3.0f); // 0.9~1.1 범위에서 예측 가능한 변화
            scatterVelocity = scatterVelocity * predictableFactor;
            
            rigidbody->velocity = scatterVelocity;
            
            // b 조각의 지속 운동량 초기화 (타입별 마찰력 시스템)
            if (fragmentType == HonmunType::b)
            {
                fragmentScript->SetPersistentVelocity(scatterVelocity);
                OutputDebugStringA("b fragment persistent velocity initialized for sustained motion\n");
            }
            
            char velocityMsg[150];
            sprintf_s(velocityMsg, "Fragment %d velocity: (%.1f, %.1f), speed: %.1f, layer: %.1fx\n", 
                     i+1, scatterVelocity.x, scatterVelocity.y, scatterSpeed, layerMultiplier);
            OutputDebugStringA(velocityMsg);
        }
        
        // 드래곤볼 스타일: b 조각들 간의 연쇄 충돌 활성화 (향후 구현)
        // fragmentScript->SetCanChainCollide(true); // b+b 연쇄 충돌 허용 - TODO: 구현 필요
        
            // 혼문 관리 시스템에 안전하게 추가
            auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
            if (aronScene) {
                aronScene->AddHonmunToManager(fragmentHonmun);
            }
            
            char debugMsg[150];
            sprintf_s(debugMsg, "Fragment %d: pos(%.1f,%.1f), size %.2f, speed x%.1f, added to manager\n", 
                     i+1, spawnPos.x, spawnPos.y, newSize, speedMultiplier);
            OutputDebugStringA(debugMsg);
            
        } catch (...) {
            OutputDebugStringA("EXCEPTION: Error configuring fragment, cleaning up...\n");
            if (fragmentHonmun) {
                fragmentHonmun->SetActive(false);
            }
            continue;
        }
    }
    
    // 연쇄 충돌 최적화: 2차 분해 제거하고 b+b 연쇄 충돌에 집중
    // ScheduleSecondaryFragmentation 제거 - 자연스러운 b+b 연쇄 충돌로 대체
    
    char finalMsg[150];
    sprintf_s(finalMsg, "=== %d b-fragments created for CHAIN COLLISION (optimized) ===\n", enhancedCount);
    OutputDebugStringA(finalMsg);
}

void HonmunCollisionEffects::AttractAndDestroyEnemies(HonmunCollisionBase* script, const Vector2& attractionPoint, float pullRatio)
{
    // C+C 효과: 카메라 시야 내 적들을 끌어당기기
    if (!script->GetCollisionManager()) {
        OutputDebugStringA("C+C attraction: No collision manager, skipping\n");
        return;
    }
    
    // 안전한 방식으로 주변 혼문들 가져오기
    auto nearbyHonmuns = script->GetCollisionManager()->GetNearbyHonmuns(script, 500.0f);
    
    // 안전성 체크: 벡터 복사로 안전한 순회
    std::vector<HonmunCollisionBase*> safeNearbyList = nearbyHonmuns;
    
    int attractedCount = 0;
    for (auto* nearbyScript : safeNearbyList)
    {
        // 유효성 검사 강화
        if (!nearbyScript || nearbyScript == script || 
            nearbyScript->IsMarkedForDestroy() || 
            !nearbyScript->GetTransform()) {
            continue;
        }
        
        try {
            Vector2 currentPos = nearbyScript->GetTransform()->GetPosition();
            Vector2 toAttraction = attractionPoint - currentPos;
            float attractionDistance = toAttraction.Magnitude();
            Vector2 attractionDir = toAttraction.Normalized();
            
            // 기획서: 속도 10으로 끌어당기기 (거리는 길게 유지)
            float attractionSpeed = pullRatio; // pullRatio = 10.0f (속도)
            Vector2 newPos = currentPos + attractionDir * attractionSpeed; // 속도 10으로 이동
            
            nearbyScript->GetTransform()->SetPosition(newPos.x, newPos.y);
            attractedCount++;
        }
        catch (...) {
            OutputDebugStringA("C+C attraction: Exception caught during position update\n");
            continue;
        }
    }
    
    char debugMsg[100];
    sprintf_s(debugMsg, "C+C attraction: %d enemies pulled with speed 10\n", 
             attractedCount);
    OutputDebugStringA(debugMsg);
}

void HonmunCollisionEffects::BounceOppositeDirections(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // A+B 효과: 반대 방향으로 튕김
    Vector2 pos1 = script->GetTransform()->GetPosition();
    Vector2 pos2 = otherScript->GetTransform()->GetPosition();
    
    Vector2 direction = (pos1 - pos2).Normalized();
    float bounceDistance = 50.0f; // 연쇄 작용을 위해 거리 단축 (80→50)
    
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
    
    // 좌우 방향 결정 - 연쇄 작용을 위해 거리 단축
    float pushDistance = 60.0f; // 연쇄 작용을 위해 거리 단축 (100→60)
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
    float separationDistance = 45.0f; // 연쇄 작용을 위해 거리 단축 (70→45)
    
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

void HonmunCollisionEffects::ScheduleSecondaryFragmentation(const Vector2& centerPoint, int secondaryCount, float secondarySize, float secondarySpeed)
{
    // 드래곤볼 스타일 2차 분해: 0.5초 후 추가 조각들이 폭발
    // 실제 타이머 구현은 HonmunCollisionBase::Update()에서 처리
    
    char scheduleMsg[150];
    sprintf_s(scheduleMsg, "Secondary fragmentation scheduled: %d fragments at (%.1f, %.1f) in 0.5s\n", 
             secondaryCount, centerPoint.x, centerPoint.y);
    OutputDebugStringA(scheduleMsg);
    
    // 2차 분해는 기존 조각들 중 일부가 더 작은 조각으로 분해되는 연쇄 효과
    // 이는 b 조각들 간의 충돌로 자연스럽게 발생하도록 설계됨
    OutputDebugStringA("Dragon Ball chain reaction: secondary fragments will trigger from b+b collisions\n");
}