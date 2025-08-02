#include "HonmunCollisionTypes.h"
#include "HonmunCollisionEffects.h"
#include "HonmunCollisionManager.h"
#include "Aron_Scene.h"  // 혼문 관리 시스템 연동
#include "../Direct2D_EngineLib/SceneManager.h"

void HonmunCollisionTypes::ProcessCollision(HonmunCollisionBase* script1, HonmunCollisionBase* script2, const ContactInfo& contact)
{
    OutputDebugStringA("=== ProcessCollision STARTED ===\n");
    
    if (!ShouldProcessCollision(script1, script2)) 
    {
        OutputDebugStringA("ShouldProcessCollision returned false, exiting\n");
        return;
    }

    HonmunType type1 = script1->GetHonmunType();
    HonmunType type2 = script2->GetHonmunType();
    
    char debugMsg[150];
    sprintf_s(debugMsg, "ProcessCollision: Type1=%d, Type2=%d\n", 
             static_cast<int>(type1), static_cast<int>(type2));
    OutputDebugStringA(debugMsg);

    // Same type collisions
    if (type1 == type2)
    {
        OutputDebugStringA("Same type collision detected\n");
        switch (type1)
        {
        case HonmunType::A:
            OutputDebugStringA("Calling HandleIgnisReaction...\n");
            HandleIgnisReaction(script1, script2);
            break;
        case HonmunType::A2:
            OutputDebugStringA("Calling Handle2AReaction...\n");
            Handle2AReaction(script1, script2);
            break;
        case HonmunType::B:
            OutputDebugStringA("Calling HandleUmbraReaction...\n");
            HandleUmbraReaction(script1, script2);
            break;
        case HonmunType::b:
            OutputDebugStringA("Calling HandleSmallBReaction...\n");
            HandleSmallBReaction(script1, script2);
            break;
        case HonmunType::C:
            OutputDebugStringA("Calling HandleDarknessReaction...\n");
            HandleDarknessReaction(script1, script2);
            break;
        case HonmunType::D:
            OutputDebugStringA("Calling HandleLunaReaction...\n");
            HandleLunaReaction(script1, script2);
            break;
        }
    }
    else
    {
        OutputDebugStringA("Mixed type collision detected\n");
        // Mixed type collision
        HandleMixedReaction(script1, script2);
    }
    
    OutputDebugStringA("=== ProcessCollision COMPLETED ===\n");
}

void HonmunCollisionTypes::HandleIgnisReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // A + A = 진짜 합체 (중간지점에 새로운 2A 생성)
    
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    OutputDebugStringA("=== A + A TRUE MERGE STARTED ===\n");
    
    // A+A 충돌은 점수 변화 없음 (0점)
    OutputDebugStringA("A+A collision: no score change (0 points)\n");
    
    // 합체 지점 계산 (두 A의 중간지점)
    Vector2 mergePoint;
    if (script->GetTransform() && otherScript->GetTransform())
    {
        Vector2 pos1 = script->GetTransform()->GetPosition();
        Vector2 pos2 = otherScript->GetTransform()->GetPosition();
        mergePoint = Vector2((pos1.x + pos2.x) * 0.5f, (pos1.y + pos2.y) * 0.5f);
        
        char posDebugMsg[100];
        sprintf_s(posDebugMsg, "A+A merge point: (%.1f, %.1f)\n", mergePoint.x, mergePoint.y);
        OutputDebugStringA(posDebugMsg);
    }
    
    // 새로운 크기 계산 (10% 증가)
    float originalSize = script->GetCurrentSize();
    float mergedSize = originalSize * 1.1f; // 10% 증가
    
    // 중간지점에 새로운 2A 생성
    auto* merged2A = currentScene->CreateObject<Honmun>(mergePoint);
    if (merged2A)
    {
        merged2A->name = "Honmun_A2_Merged";
        merged2A->SetHonmunType(HonmunType::A2); // 명확하게 A2 타입으로
        merged2A->SetSize(mergedSize);
        
        // 2A의 핵심: HP가 1
        merged2A->SetHP(1);
        
        // 충돌 스크립트 강제 설정
        auto* merged2AScript = merged2A->GetComponent<HonmunCollisionBase>();
        if (!merged2AScript) {
            merged2AScript = merged2A->AddComponent<HonmunCollisionBase>();
        }
        merged2AScript->SetCurrentSize(mergedSize);
        merged2AScript->SetHealth(1); // 2A는 HP 1
        merged2AScript->SetHonmunType(HonmunType::A2); // 스크립트도 A2로
        
        // 콜라이더 트리거 설정 강제 확인
        auto* collider = merged2A->GetComponent<CircleCollider>();
        if (collider) {
            collider->isTrigger = true;
            OutputDebugStringA("A2 collider set to trigger mode\n");
        }
        
        // Rigidbody 물리 설정
        auto* rb = merged2A->GetComponent<Rigidbody>();
        if (rb) {
            rb->isKinematic = false;  // 충돌 감지를 위해 키네마틱 해제
            rb->useGravity = false;   // 중력은 비활성화 유지
            OutputDebugStringA("A2 rigidbody physics enabled for collision\n");
        }
        
        OutputDebugStringA("SUCCESS: New 2A created at merge point!\n");
        
        char debugMsg[200];
        sprintf_s(debugMsg, "2A created: Size %.2f (10%% increase), HP=1, Position(%.1f, %.1f), Type=%d\n", 
                 mergedSize, mergePoint.x, mergePoint.y, static_cast<int>(merged2A->GetHonmunType()));
        OutputDebugStringA(debugMsg);
        
        // A2 타입 설정 재확인
        char typeDebugMsg[100];
        sprintf_s(typeDebugMsg, "A2 final type check: Honmun=%d, Script=%d\n", 
                 static_cast<int>(merged2A->GetHonmunType()), 
                 static_cast<int>(merged2AScript->GetHonmunType()));
        OutputDebugStringA(typeDebugMsg);
        
        // 혼문 관리 시스템에 추가
        auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
        if (aronScene) {
            aronScene->AddHonmunToManager(merged2A);
            OutputDebugStringA("A2 added to HonmunManager\n");
        }
    }
    else
    {
        OutputDebugStringA("ERROR: Failed to create merged 2A!\n");
    }
    
    // 원본 두 A 모두 파괴 (관리 시스템에서도 제거)
    OutputDebugStringA("Destroying both original A objects...\n");
    if (aronScene) {
        aronScene->RemoveHonmunFromManager(script->GetHonmun());
        aronScene->RemoveHonmunFromManager(otherScript->GetHonmun());
        OutputDebugStringA("Original A objects removed from HonmunManager\n");
    }
    script->DestroyThis();
    otherScript->DestroyThis();
    
    OutputDebugStringA("=== A + A TRUE MERGE COMPLETED ===\n");
}

void HonmunCollisionTypes::Handle2AReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // 2A + 2A = 둘 다 HP 1이므로 둘 다 소멸, 6점 획득
    
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    OutputDebugStringA("=== 2A + 2A COLLISION STARTED ===\n");
    OutputDebugStringA("2A + 2A collision - both destroyed for 6 points!\n");
    
    // 2A+2A 충돌: 둘 다 소멸하므로 6점 (2A=3점 × 2개)
    if (aronScene) 
    {
        aronScene->AddScore(6); // 2A × 2 = 3점 × 2 = 6점
        OutputDebugStringA("2A+2A destruction: +6 points (3pts × 2)\n");
    }
    
    // 둘 다 즉시 파괴 (HP 1 + HP 1 = 소멸)
    OutputDebugStringA("Destroying both 2A objects...\n");
    if (aronScene) {
        aronScene->RemoveHonmunFromManager(script->GetHonmun());
        aronScene->RemoveHonmunFromManager(otherScript->GetHonmun());
    }
    script->DestroyThis();
    otherScript->DestroyThis();
    
    OutputDebugStringA("=== 2A + 2A COLLISION COMPLETED ===\n");
}

void HonmunCollisionTypes::HandleSmallBReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // b + b = 둘 다 HP 1이므로 둘 다 소멸, 2점 획득
    
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    OutputDebugStringA("=== b + b COLLISION STARTED ===\n");
    OutputDebugStringA("b + b collision - both destroyed for 2 points!\n");
    
    // b+b 충돌: 둘 다 소멸하므로 2점 (b=1점 × 2개)
    if (aronScene) 
    {
        aronScene->AddScore(2); // b × 2 = 1점 × 2 = 2점
        OutputDebugStringA("b+b destruction: +2 points (1pt × 2)\n");
    }
    
    // 둘 다 즉시 파괴 (HP 1 + HP 1 = 소멸)
    OutputDebugStringA("Destroying both b objects...\n");
    if (aronScene) {
        aronScene->RemoveHonmunFromManager(script->GetHonmun());
        aronScene->RemoveHonmunFromManager(otherScript->GetHonmun());
    }
    script->DestroyThis();
    otherScript->DestroyThis();
    
    OutputDebugStringA("=== b + b COLLISION COMPLETED ===\n");
}

void HonmunCollisionTypes::HandleUmbraReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // B + B = 4개로 분열 (30% 크기 감소, 20% 낙하속도 증가)
    
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    OutputDebugStringA("=== B + B COLLISION STARTED ===\n");
    OutputDebugStringA("B + B collision - splitting into 4 pieces with 30% size decrease, 20% speed increase!\n");
    
    // B+B 충돌은 점수 변화 없음 (0점)
    OutputDebugStringA("B+B collision: no score change (0 points)\n");
    
    // 충돌 지점 계산
    Vector2 collisionPoint;
    if (script->GetTransform() && otherScript->GetTransform())
    {
        Vector2 pos1 = script->GetTransform()->GetPosition();
        Vector2 pos2 = otherScript->GetTransform()->GetPosition();
        collisionPoint = Vector2((pos1.x + pos2.x) * 0.5f, (pos1.y + pos2.y) * 0.5f);
        
        char posDebugMsg[100];
        sprintf_s(posDebugMsg, "B+B collision point: (%.1f, %.1f)\n", collisionPoint.x, collisionPoint.y);
        OutputDebugStringA(posDebugMsg);
    }
    
    // 4개 조각 생성 BEFORE destroying originals
    if (script->GetCollisionEffects())
    {
        // 새로운 크기 계산 (30% 감소)
        float originalSize = script->GetCurrentSize();
        float newSize = originalSize * 0.7f; // 30% 감소
        
        OutputDebugStringA("Calling CreateSplitFragments with count=4...\n");
        script->GetCollisionEffects()->CreateSplitFragments(script, collisionPoint, 4, newSize, 1.2f); // 20% 속도 증가
        OutputDebugStringA("CreateSplitFragments call completed.\n");
        
        char debugMsg[150];
        sprintf_s(debugMsg, "B+B split: 4 fragments requested, Size %.2f (%.1f%% decrease), Speed increased by 20%%\n", 
                 newSize, (1.0f - (newSize/originalSize)) * 100.0f);
        OutputDebugStringA(debugMsg);
    }
    else
    {
        OutputDebugStringA("ERROR: script->GetCollisionEffects() is null!\n");
    }
    
    // 원본 객체들 파괴 (fragment 생성 후)
    OutputDebugStringA("Destroying original B objects...\n");
    if (aronScene) {
        aronScene->RemoveHonmunFromManager(script->GetHonmun());
        aronScene->RemoveHonmunFromManager(otherScript->GetHonmun());
    }
    script->DestroyThis();
    otherScript->DestroyThis();
    
    OutputDebugStringA("=== B + B COLLISION COMPLETED ===\n");
}

void HonmunCollisionTypes::HandleDarknessReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // C + C = 카메라 시점 내 적들을 충돌지점으로 끌어당기고 파괴됨 (밀리는거리의 1/3)
    
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    OutputDebugStringA("=== C + C COLLISION STARTED ===\n");
    OutputDebugStringA("C + C collision - attracting camera-view enemies and destroying!\n");
    
    // 충돌 지점 계산
    Vector2 attractionPoint;
    if (script->GetTransform() && otherScript->GetTransform())
    {
        Vector2 pos1 = script->GetTransform()->GetPosition();
        Vector2 pos2 = otherScript->GetTransform()->GetPosition();
        attractionPoint = Vector2((pos1.x + pos2.x) * 0.5f, (pos1.y + pos2.y) * 0.5f);
    }
    
    // C+C 충돌 점수 부여 (1점)
    if (aronScene)
    {
        aronScene->AddScore(1); // C+C 충돌 (1점)
        
        char debugMsg[50];
        sprintf_s(debugMsg, "C+C attraction: +1 point\n");
        OutputDebugStringA(debugMsg);
    }
    
    // 주변 적들 끌어당기기 (밀리는 거리의 1/3)
    if (script->GetCollisionEffects())
    {
        script->GetCollisionEffects()->AttractAndDestroyEnemies(script, attractionPoint, 1.0f/3.0f); // 1/3 거리만큼 당김
        
        char debugMsg[100];
        sprintf_s(debugMsg, "C+C attraction: Pulling enemies to (%.1f, %.1f) by 1/3 distance\n", 
                 attractionPoint.x, attractionPoint.y);
        OutputDebugStringA(debugMsg);
    }
    
    // 원본 C 객체들 파괴
    OutputDebugStringA("Destroying both C objects...\n");
    if (aronScene) {
        aronScene->RemoveHonmunFromManager(script->GetHonmun());
        aronScene->RemoveHonmunFromManager(otherScript->GetHonmun());
    }
    script->DestroyThis();
    otherScript->DestroyThis();
    
    OutputDebugStringA("=== C + C COLLISION COMPLETED ===\n");
}

void HonmunCollisionTypes::HandleLunaReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // D + D 충돌 = 즉시 파괴 (체력 감소 없이)
    
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    OutputDebugStringA("D + D collision - immediate destruction!\n");
    
    // D+D 충돌은 점수 변화 없음 (0점)
    OutputDebugStringA("D+D collision: no score change (0 points)\n");
    
    // D+D 파괴 시 강한 충격파 효과
    ApplyCollisionForce(script, otherScript, 250.0f); // 매우 강한 충격
    
    // 둘 다 즉시 파괴 (체력 감소 없이)
    if (aronScene) {
        aronScene->RemoveHonmunFromManager(script->GetHonmun());
        aronScene->RemoveHonmunFromManager(otherScript->GetHonmun());
    }
    script->DestroyThis();
    otherScript->DestroyThis();
}

void HonmunCollisionTypes::HandleMixedReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // 서로 다른 타입 간의 충돌 처리
    HonmunType type1 = script->GetHonmunType();
    HonmunType type2 = otherScript->GetHonmunType();
    
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    // D와의 충도은 모두 즉시 파괴 (체력 감소 없이)
    if (type1 == HonmunType::D || type2 == HonmunType::D)
    {
        OutputDebugStringA("Mixed collision with D - immediate destruction!\n");
        
        // D와의 모든 충돌은 점수 변화 없음 (0점)
        OutputDebugStringA("D mixed collision: no score change (0 points)\n");
        
        // 즉시 파괴 (체력 감소 없이)
        if (aronScene) {
            aronScene->RemoveHonmunFromManager(script->GetHonmun());
            aronScene->RemoveHonmunFromManager(otherScript->GetHonmun());
        }
        script->DestroyThis();
        otherScript->DestroyThis();
        return;
    }
    
    // A + B / B + A = 충돌 진행방향과 반대방향으로 서로 튕겨냄
    if ((type1 == HonmunType::A && type2 == HonmunType::B) || 
        (type1 == HonmunType::B && type2 == HonmunType::A))
    {
        OutputDebugStringA("A + B collision - bouncing in opposite directions!\n");
        
        // 튕김 효과: 서로 반대 방향으로 밀려남
        ApplyOppositeForces(script, otherScript, 500.0f); // 매우 강한 밀림 효과로 증가
        
        if (aronScene) 
        {
            aronScene->AddScore(1);
            OutputDebugStringA("A+B bounce: +1 point\n");
        }
    }
    // A + C / C + A = 충돌방향과 관계없이 좌, 우로 밀어냄
    else if ((type1 == HonmunType::A && type2 == HonmunType::C) || 
             (type1 == HonmunType::C && type2 == HonmunType::A))
    {
        OutputDebugStringA("A + C collision - pushing left/right!\n");
        
        // 좌우 밀림 효과
        ApplyLeftRightPush(script, otherScript, 250.0f); // 강한 좌우 밀림으로 증가
        
        if (aronScene) 
        {
            aronScene->AddScore(1);
            OutputDebugStringA("A+C push: +1 point\n");
        }
    }
    // B + C / C + B = 서로 관통하지만 겹쳐지지는 않음
    else if ((type1 == HonmunType::B && type2 == HonmunType::C) || 
             (type1 == HonmunType::C && type2 == HonmunType::B))
    {
        OutputDebugStringA("B + C collision - penetrating without overlap!\n");
        
        // 관통 효과 (약간의 분리만)
        ApplyPenetration(script, otherScript);
        
        if (aronScene) 
        {
            aronScene->AddScore(1);
            OutputDebugStringA("B+C penetration: +1 point\n");
        }
    }
}

bool HonmunCollisionTypes::ShouldProcessCollision(HonmunCollisionBase* script1, HonmunCollisionBase* script2)
{
    // Only check for null - processing prevention is handled in OnTriggerEnter
    if (!script1 || !script2)
    {
        OutputDebugStringA("ShouldProcessCollision: One or both scripts are null\n");
        return false;
    }
    
    OutputDebugStringA("ShouldProcessCollision: OK to process collision\n");
    return true;
}

void HonmunCollisionTypes::ApplyHealthChange(HonmunCollisionBase* script, int healthChange)
{
    int newHealth = script->GetHealth() + healthChange;
    script->SetHealth(newHealth);
}

void HonmunCollisionTypes::ApplyCollisionForce(HonmunCollisionBase* script, HonmunCollisionBase* otherScript, float force)
{
    // 충돌 방향 계산
    auto* transform1 = script->GetTransform();
    auto* transform2 = otherScript->GetTransform();
    
    if (!transform1 || !transform2) return;
    
    Vector2 pos1 = transform1->GetPosition();
    Vector2 pos2 = transform2->GetPosition();
    
    // 충돌 방향 벡터 (script가 밀려나는 방향)
    Vector2 forceDirection = (pos1 - pos2).Normalized();
    
    // 충돌 테스트용: 키네마틱 모드 유지하고 직접 위치 이동으로 변경
    Vector2 displacement = forceDirection * (force * 0.2f); // 스케일 조정
    Vector2 newPos = pos1 + displacement;
    transform1->SetPosition(newPos.x, newPos.y);
    
    char debugMsg[100];
    sprintf_s(debugMsg, "Applied kinematic displacement: (%.1f, %.1f)\n", 
             displacement.x, displacement.y);
    OutputDebugStringA(debugMsg);
}

void HonmunCollisionTypes::ApplyOppositeForces(HonmunCollisionBase* script1, HonmunCollisionBase* script2, float force)
{
    // 서로 반대 방향으로 튕겨냄 - 키네마틱 모드 유지 버전
    auto* transform1 = script1->GetTransform();
    auto* transform2 = script2->GetTransform();
    
    if (!transform1 || !transform2) return;
    
    Vector2 pos1 = transform1->GetPosition();
    Vector2 pos2 = transform2->GetPosition();
    
    // 충돌 방향 벡터
    Vector2 direction = (pos1 - pos2).Normalized();
    
    // 키네마틱 모드에서 즉시 위치 변경으로 분리
    float displacement = force * 0.4f; // 이동 거리
    
    // 즉시 위치 분리
    Vector2 newPos1 = pos1 + direction * displacement;
    Vector2 newPos2 = pos2 - direction * displacement;
    transform1->SetPosition(newPos1.x, newPos1.y);
    transform2->SetPosition(newPos2.x, newPos2.y);
    
    char debugMsg[100];
    sprintf_s(debugMsg, "Applied kinematic opposite bounce: %.1f\n", force);
    OutputDebugStringA(debugMsg);
}

void HonmunCollisionTypes::ApplyLeftRightPush(HonmunCollisionBase* script1, HonmunCollisionBase* script2, float force)
{
    // 충돌방향과 관계없이 좌, 우로 밀어냄 - 키네마틱 모드 유지 버전
    auto* transform1 = script1->GetTransform();
    auto* transform2 = script2->GetTransform();
    
    if (!transform1 || !transform2) return;
    
    Vector2 pos1 = transform1->GetPosition();
    Vector2 pos2 = transform2->GetPosition();
    
    // 좌우 방향 벡터 (왼쪽과 오른쪽)
    Vector2 leftDirection(-1.0f, 0.0f);
    Vector2 rightDirection(1.0f, 0.0f);
    
    // 키네마틱 모드에서 즉시 위치 변경
    float displacement = force * 0.5f;
    Vector2 newPos1 = pos1 + leftDirection * displacement;
    Vector2 newPos2 = pos2 + rightDirection * displacement;
    transform1->SetPosition(newPos1.x, newPos1.y);
    transform2->SetPosition(newPos2.x, newPos2.y);
    
    char debugMsg[100];
    sprintf_s(debugMsg, "Applied kinematic left-right push: %.1f\n", force);
    OutputDebugStringA(debugMsg);
}

void HonmunCollisionTypes::ApplyPenetration(HonmunCollisionBase* script1, HonmunCollisionBase* script2)
{
    // B+C 관통: 겹치지 않도록 확실한 분리 - 키네마틱 모드 유지
    auto* transform1 = script1->GetTransform();
    auto* transform2 = script2->GetTransform();
    
    if (!transform1 || !transform2) return;
    
    Vector2 pos1 = transform1->GetPosition();
    Vector2 pos2 = transform2->GetPosition();
    
    // 충돌 방향 및 거리 계산
    Vector2 direction = (pos1 - pos2);
    float distance = direction.Magnitude();
    
    if (distance < 80.0f) // 콜라이더가 겹치는 상황
    {
        direction = direction.Normalized();
        
        // 즉시 분리 - 최소 거리 80픽셀 확보
        float separationDistance = 80.0f - distance;
        Vector2 separation = direction * (separationDistance * 0.5f);
        
        // 양쪽으로 분리 (키네마틱 모드 유지)
        Vector2 newPos1 = pos1 + separation;
        Vector2 newPos2 = pos2 - separation;
        
        transform1->SetPosition(newPos1.x, newPos1.y);
        transform2->SetPosition(newPos2.x, newPos2.y);
        
        char debugMsg[100];
        sprintf_s(debugMsg, "B+C kinematic penetration: separated by %.1f pixels\n", separationDistance);
        OutputDebugStringA(debugMsg);
    }
    else
    {
        OutputDebugStringA("B+C penetration: objects already separated\n");
    }
}