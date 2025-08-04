#include "HonmunCollisionTypes.h"
#include "HonmunCollisionEffects.h"
#include "HonmunCollisionManager.h"
#include "Aron_Scene.h"  // 혼문 관리 시스템 연동
#include "../Direct2D_EngineLib/SceneManager.h"

void HonmunCollisionTypes::ProcessCollision(HonmunCollisionBase* script1, HonmunCollisionBase* script2, const ContactInfo& contact)
{
    OutputDebugStringA("=== ProcessCollision STARTED ===\n");
    
    // Enhanced safety checks with comprehensive validation
    if (!script1 || !script2) {
        OutputDebugStringA("ERROR: Null script detected in ProcessCollision\n");
        return;
    }
    
    // Check if objects are still valid before processing
    if (script1->IsMarkedForDestroy() || script2->IsMarkedForDestroy()) {
        OutputDebugStringA("WARNING: One or both objects already marked for destruction\n");
        return;
    }
    
    // Validate game objects and components
    if (!script1->gameObject || !script2->gameObject ||
        !script1->GetHonmun() || !script2->GetHonmun() ||
        !script1->GetTransform() || !script2->GetTransform()) {
        OutputDebugStringA("ERROR: Invalid game objects or components detected\n");
        return;
    }
    
    if (!ShouldProcessCollision(script1, script2)) 
    {
        OutputDebugStringA("ShouldProcessCollision returned false, exiting\n");
        return;
    }

    // Safe type and health access with additional validation
    HonmunType type1, type2;
    int hp1, hp2;
    
    try {
        type1 = script1->GetHonmunType();
        type2 = script2->GetHonmunType();
        hp1 = script1->GetHealth();
        hp2 = script2->GetHealth();
        
        // Validate enum values are within range
        if (static_cast<int>(type1) < 0 || static_cast<int>(type1) > 5 ||
            static_cast<int>(type2) < 0 || static_cast<int>(type2) > 5) {
            OutputDebugStringA("ERROR: Invalid HonmunType enum values detected\n");
            return;
        }
    } catch (...) {
        OutputDebugStringA("ERROR: Exception while accessing type/health data\n");
        return;
    }
    
    char debugMsg[200];
    sprintf_s(debugMsg, "ProcessCollision: Type1=%d(HP=%d), Type2=%d(HP=%d)\n", 
             static_cast<int>(type1), hp1, static_cast<int>(type2), hp2);
    OutputDebugStringA(debugMsg);

    // 기획서 기반 충돌 처리: 체력 1 상호작용X 체크
    if (hp1 == 1 && hp2 == 1)
    {
        OutputDebugStringA("Both HP=1, no interaction\n");
        return;
    }

    // Same type collisions (동일속성 충돌)
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
    // A + A = 기획서 기반: 합쳐짐. 합쳐진 2A는 HP 1, 충돌 시 0점
    
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    OutputDebugStringA("=== A + A MERGE (기획서 기반) STARTED ===\n");
    
    // 기획서: A+A 충돌은 점수 변화 없음 (0점)
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
    
    // 중간지점에 새로운 2A 생성 (안전한 객체 생성)
    auto* merged2A = currentScene->CreateObject<Honmun>(mergePoint);
    if (!merged2A)
    {
        OutputDebugStringA("CRITICAL ERROR: Failed to create merged 2A object!\n");
        return;
    }
    
    try {
        merged2A->name = "Honmun_A2_Merged";
        merged2A->SetHonmunType(HonmunType::A2); // 명확하게 A2 타입으로
        merged2A->SetSize(mergedSize);
        
        // 2A의 핵심: HP가 1
        merged2A->SetHP(1);
        
        // 충돌 스크립트 안전한 설정
        auto* merged2AScript = merged2A->GetComponent<HonmunCollisionBase>();
        if (!merged2AScript) {
            merged2AScript = merged2A->AddComponent<HonmunCollisionBase>();
            if (!merged2AScript) {
                OutputDebugStringA("CRITICAL ERROR: Failed to add collision script to 2A!\n");
                // 생성된 객체 정리
                merged2A->SetActive(false);
                return;
            }
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
        
        // 혼문 관리 시스템에 안전하게 추가 (프레임 지연)
        auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
        if (aronScene) {
            // 다음 프레임에 추가하도록 지연 (초기화 완료 보장)
            try {
                aronScene->AddHonmunToManager(merged2A);
                OutputDebugStringA("A2 added to HonmunManager successfully\n");
            } catch (...) {
                OutputDebugStringA("EXCEPTION: Failed to add A2 to manager\n");
                // 실패 시 A2 객체 정리
                if (merged2A) {
                    merged2A->SetActive(false);
                }
                return;
            }
        } else {
            OutputDebugStringA("WARNING: Failed to cast to Aron_Scene for A2 manager addition\n");
        }
        
        OutputDebugStringA("SUCCESS: New 2A created and configured successfully!\n");
        
    } catch (...) {
        OutputDebugStringA("EXCEPTION: Error configuring merged 2A object, cleaning up...\n");
        if (merged2A) {
            merged2A->SetActive(false);
        }
        return;
    }
    
    // 원본 두 A 모두 안전하게 파괴 (지연된 파괴)
    OutputDebugStringA("Safely destroying both original A objects...\n");
    try {
        // 먼저 충돌 처리 비활성화
        script->SetProcessingReaction(true);
        otherScript->SetProcessingReaction(true);
        
        // 관리자에서 먼저 제거
        if (aronScene) {
            try {
                if (script->GetHonmun()) {
                    aronScene->RemoveHonmunFromManager(script->GetHonmun());
                }
                if (otherScript->GetHonmun()) {
                    aronScene->RemoveHonmunFromManager(otherScript->GetHonmun());
                }
                OutputDebugStringA("Original A objects removed from HonmunManager\n");
            } catch (...) {
                OutputDebugStringA("EXCEPTION: Error removing from manager\n");
            }
        }
        
        // 마크 후 안전하게 파괴 (다음 프레임에 실제 파괴)
        if (!script->IsMarkedForDestroy()) {
            script->DestroyThis();
            OutputDebugStringA("First A object marked for destruction\n");
        }
        if (!otherScript->IsMarkedForDestroy()) {
            otherScript->DestroyThis();
            OutputDebugStringA("Second A object marked for destruction\n");
        }
        
        OutputDebugStringA("Original A objects marked for destruction safely\n");
    } catch (...) {
        OutputDebugStringA("EXCEPTION: Error during A object destruction, continuing...\n");
    }
    
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
    // b + b = 드래곤볼/포켓볼 스타일 연쇄 분해 충돌 + 둘 다 소멸, 2점 획득
    
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    OutputDebugStringA("=== b + b CHAIN COLLISION STARTED (Dragon Ball style) ===\n");
    OutputDebugStringA("b + b collision - chain reaction breakup + both destroyed for 2 points!\n");
    
    // 연쇄 충돌 지점 계산
    Vector2 chainCollisionPoint;
    if (script->GetTransform() && otherScript->GetTransform())
    {
        Vector2 pos1 = script->GetTransform()->GetPosition();
        Vector2 pos2 = otherScript->GetTransform()->GetPosition();
        chainCollisionPoint = Vector2((pos1.x + pos2.x) * 0.5f, (pos1.y + pos2.y) * 0.5f);
        
        char posDebugMsg[100];
        sprintf_s(posDebugMsg, "b+b chain collision point: (%.1f, %.1f)\n", chainCollisionPoint.x, chainCollisionPoint.y);
        OutputDebugStringA(posDebugMsg);
    }
    
    // 드래곤볼 스타일 연쇄 분해: 작은 조각들도 더 작은 파편으로 산산조각
    if (script->GetCollisionEffects())
    {
        float smallFragmentSize = script->GetCurrentSize() * 0.5f; // 50% 더 작게
        
        OutputDebugStringA("Creating micro-fragments from b+b chain collision...\n");
        // 2개의 b가 6개의 micro-fragment로 분해 (더 화려한 효과)
        script->GetCollisionEffects()->CreateSplitFragments(script, chainCollisionPoint, 6, smallFragmentSize, 1.5f); // 50% 더 빠르게
        
        char debugMsg[150];
        sprintf_s(debugMsg, "b+b chain split: 6 micro-fragments created, Size %.2f (50%% smaller), Speed increased by 50%%\n", 
                 smallFragmentSize);
        OutputDebugStringA(debugMsg);
        
        // 추가 시각 효과: 강한 충격파 (포켓볼 스타일)
        ApplyCollisionForce(script, otherScript, 400.0f); // 일반보다 강한 충격
    }
    else
    {
        OutputDebugStringA("ERROR: script->GetCollisionEffects() is null for b+b chain collision!\n");
    }
    
    // b+b 충돌: 둘 다 소멸하므로 2점 (b=1점 × 2개)
    if (aronScene) 
    {
        aronScene->AddScore(2); // b × 2 = 1점 × 2 = 2점
        OutputDebugStringA("b+b chain destruction: +2 points (1pt × 2) + spectacular breakup effect\n");
    }
    
    // 둘 다 즉시 파괴 (HP 1 + HP 1 = 소멸)
    OutputDebugStringA("Destroying both b objects after chain reaction...\n");
    if (aronScene) {
        aronScene->RemoveHonmunFromManager(script->GetHonmun());
        aronScene->RemoveHonmunFromManager(otherScript->GetHonmun());
    }
    script->DestroyThis();
    otherScript->DestroyThis();
    
    OutputDebugStringA("=== b + b CHAIN COLLISION COMPLETED (Dragon Ball effect applied) ===\n");
}

void HonmunCollisionTypes::HandleUmbraReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // B + B = 기획서 기반: 체력에 따라 분열 또는 체력 감소
    
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    int hp1 = script->GetHealth();
    int hp2 = otherScript->GetHealth();
    
    OutputDebugStringA("=== B + B COLLISION STARTED ===\n");
    
    // 기획서: B1 + B1은 분열 없이 체력 감소만
    if (hp1 == 1 && hp2 == 1)
    {
        OutputDebugStringA("B1 + B1: Health decrease only, no split\n");
        
        // 둘 다 체력 감소 (-1)
        script->SetHealth(0);  // B1이 체력 감소하면 파괴
        otherScript->SetHealth(0);  // B1이 체력 감소하면 파괴
        
        // 점수 +2 (B1 파괴 시 2점)
        if (aronScene) {
            aronScene->AddScore(2);
            OutputDebugStringA("B1+B1 collision: +2 points (both destroyed)\n");
        }
        
        // 파괴 처리
        if (aronScene) {
            aronScene->RemoveHonmunFromManager(script->GetHonmun());
            aronScene->RemoveHonmunFromManager(otherScript->GetHonmun());
        }
        script->DestroyThis();
        otherScript->DestroyThis();
        
        OutputDebugStringA("=== B1 + B1 COLLISION COMPLETED ===\n");
        return;
    }
    
    // 기획서: B3, B2가 포함된 경우 분열 (b x4)
    OutputDebugStringA("B + B collision - splitting into 4 pieces (b type)\n");
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
    // C + C = 기획서 기반: 체력과 상관없이 파괴, C+C 충돌 시 3점 (기획서 C+C 표기 변경에 따라)
    
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    OutputDebugStringA("=== C + C DESTRUCTION (기획서 기반) STARTED ===\n");
    OutputDebugStringA("C + C collision - immediate destruction regardless of HP!\n");
    
    // 충돌 지점 계산
    Vector2 attractionPoint;
    if (script->GetTransform() && otherScript->GetTransform())
    {
        Vector2 pos1 = script->GetTransform()->GetPosition();
        Vector2 pos2 = otherScript->GetTransform()->GetPosition();
        attractionPoint = Vector2((pos1.x + pos2.x) * 0.5f, (pos1.y + pos2.y) * 0.5f);
    }
    
    // 기획서: C+C 충돌 점수 3점 (체력 무관 즉시 파괴)
    if (aronScene)
    {
        aronScene->AddScore(3); // 기획서의 C+C 충돌 점수
        
        char debugMsg[50];
        sprintf_s(debugMsg, "C+C destruction: +3 points\n");
        OutputDebugStringA(debugMsg);
    }
    
    // 주변 적들 끌어당기기 (밀리는 거리의 1/3)
    if (script->GetCollisionEffects())
    {
        script->GetCollisionEffects()->AttractAndDestroyEnemies(script, attractionPoint, 10.0f); // 밀리는 속도 10으로 복원
        
        char debugMsg[100];
        sprintf_s(debugMsg, "C+C attraction: Pulling enemies to (%.1f, %.1f) with speed 10\n", 
                 attractionPoint.x, attractionPoint.y);
        OutputDebugStringA(debugMsg);
    }
    
    // 원본 C 객체들 안전하게 파괴 (Manager에서 먼저 제거 후 파괴)
    OutputDebugStringA("Destroying both C objects safely...\n");
    
    // 1단계: Manager에서 제거
    if (aronScene) {
        if (script->GetHonmun()) {
            aronScene->RemoveHonmunFromManager(script->GetHonmun());
        }
        if (otherScript->GetHonmun()) {
            aronScene->RemoveHonmunFromManager(otherScript->GetHonmun());
        }
        OutputDebugStringA("C objects removed from HonmunManager\n");
    }
    
    // 2단계: CollisionManager에서 안전하게 해제 (각 객체의 manager를 사용)
    if (script->GetCollisionManager()) {
        script->GetCollisionManager()->UnregisterHonmun(script);
    }
    if (otherScript->GetCollisionManager()) {
        otherScript->GetCollisionManager()->UnregisterHonmun(otherScript);
    }
    
    // 3단계: 파괴 마킹 (실제 파괴는 엔진에서 안전하게 처리)
    if (!script->IsMarkedForDestroy()) {
        script->DestroyThis();
        OutputDebugStringA("First C object marked for destruction\n");
    }
    if (!otherScript->IsMarkedForDestroy()) {
        otherScript->DestroyThis();
        OutputDebugStringA("Second C object marked for destruction\n");
    }
    
    OutputDebugStringA("=== C + C COLLISION COMPLETED ===\n");
}

void HonmunCollisionTypes::HandleLunaReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // D + D 충돌 = 기획서에는 명시되지 않음, 기존 로직 유지
    
    OutputDebugStringA("=== D + D COLLISION STARTED ===\n");
    
    // Null pointer safety checks before any processing
    if (!script || !otherScript || !script->GetHonmun() || !otherScript->GetHonmun())
    {
        OutputDebugStringA("ERROR: Null pointer detected in D+D collision, aborting\n");
        return;
    }
    
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    OutputDebugStringA("D + D collision - immediate destruction!\n");
    
    // D+D 충돌은 점수 변화 없음 (0점) - 기획서에 명시되지 않아 기존 로직 유지
    OutputDebugStringA("D+D collision: no score change (0 points)\n");
    
    // D+D 파괴 시 강한 충격파 효과 (객체가 유효할 때만)
    if (script->GetTransform() && otherScript->GetTransform())
    {
        ApplyCollisionForce(script, otherScript, 250.0f); // 매우 강한 충격
        OutputDebugStringA("D+D collision force applied\n");
    }
    
    // 안전한 순서로 파괴: 관리자에서 제거 → 마크 → 실제 파괴
    Honmun* honmun1 = script->GetHonmun();
    Honmun* honmun2 = otherScript->GetHonmun();
    
    OutputDebugStringA("Removing D objects from manager...\n");
    if (aronScene && honmun1 && honmun2) {
        aronScene->RemoveHonmunFromManager(honmun1);
        aronScene->RemoveHonmunFromManager(honmun2);
        OutputDebugStringA("D objects removed from manager\n");
    }
    
    // 마크 후 안전하게 파괴
    OutputDebugStringA("Marking D objects for destruction...\n");
    if (script && !script->IsMarkedForDestroy()) {
        script->DestroyThis();
        OutputDebugStringA("First D object marked for destruction\n");
    }
    if (otherScript && !otherScript->IsMarkedForDestroy()) {
        otherScript->DestroyThis();
        OutputDebugStringA("Second D object marked for destruction\n");
    }
    
    OutputDebugStringA("=== D + D COLLISION COMPLETED ===\n");
}

void HonmunCollisionTypes::HandleMixedReaction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript)
{
    // 서로 다른 타입 간의 충돌 처리 - 기획서 기반
    HonmunType type1 = script->GetHonmunType();
    HonmunType type2 = otherScript->GetHonmunType();
    int hp1 = script->GetHealth();
    int hp2 = otherScript->GetHealth();
    
    auto* currentScene = SceneManager::Get().GetCurrentScene();
    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
    
    // A+B 충돌 세부 추적을 위한 상세 로그
    bool isABCollision = ((type1 == HonmunType::A || type1 == HonmunType::A2) && 
                          (type2 == HonmunType::B || type2 == HonmunType::b)) ||
                         ((type1 == HonmunType::B || type1 == HonmunType::b) && 
                          (type2 == HonmunType::A || type2 == HonmunType::A2));
    
    char debugMsg[200];
    sprintf_s(debugMsg, "*** MIXED COLLISION %s: Type%d(HP%d) vs Type%d(HP%d) ***\n", 
             isABCollision ? "[A+B]" : "[OTHER]",
             static_cast<int>(type1), hp1, static_cast<int>(type2), hp2);
    OutputDebugStringA(debugMsg);
    
    // D와의 충돌 처리
    if (type1 == HonmunType::D || type2 == HonmunType::D)
    {
        HandleDMixedCollision(script, otherScript, aronScene);
        return;
    }
    
    // 기획서의 점수 시스템 적용
    bool shouldAddScore = false;
    int scoreToAdd = 0;
    bool shouldDestroyScript = false;
    bool shouldDestroyOther = false;
    int newHp1 = hp1;
    int newHp2 = hp2;
    
    // 기획서 정밀한 혼합 충돌 점수 테이블 적용
    scoreToAdd = GetMixedCollisionScore(type1, hp1, type2, hp2);
    if (scoreToAdd != 0)
    {
        shouldAddScore = true;
        newHp1 = hp1 - 1;  // 기본적으로 체력 -1
        newHp2 = hp2 - 1;
        
        char mixedMsg[200];
        sprintf_s(mixedMsg, "Mixed collision: Type%d(HP%d→%d) vs Type%d(HP%d→%d), Score: +%d\n", 
                 static_cast<int>(type1), hp1, newHp1, static_cast<int>(type2), hp2, newHp2, scoreToAdd);
        OutputDebugStringA(mixedMsg);
    }
    else
    {
        // 점수가 0인 경우에도 체력은 감소 (A+B 기본 상호작용)
        if ((type1 == HonmunType::A || type1 == HonmunType::A2) && 
            (type2 == HonmunType::B || type2 == HonmunType::b))
        {
            newHp1 = hp1 - 1;
            newHp2 = hp2 - 1;
            
            char noScoreMsg[200];
            sprintf_s(noScoreMsg, "A+B collision (no score): Type%d(HP%d→%d) vs Type%d(HP%d→%d)\n", 
                     static_cast<int>(type1), hp1, newHp1, static_cast<int>(type2), hp2, newHp2);
            OutputDebugStringA(noScoreMsg);
        }
        else if ((type1 == HonmunType::B || type1 == HonmunType::b) && 
                 (type2 == HonmunType::A || type2 == HonmunType::A2))
        {
            newHp1 = hp1 - 1;
            newHp2 = hp2 - 1;
            
            char noScoreMsg[200];
            sprintf_s(noScoreMsg, "B+A collision (no score): Type%d(HP%d→%d) vs Type%d(HP%d→%d)\n", 
                     static_cast<int>(type1), hp1, newHp1, static_cast<int>(type2), hp2, newHp2);
            OutputDebugStringA(noScoreMsg);
        }
    }
    
    // 점수 및 체력 변화 적용 (중복 방지)
    bool scoreAlreadyApplied = false;
    if (shouldAddScore && aronScene)
    {
        aronScene->AddScore(scoreToAdd);
        scoreAlreadyApplied = true;
        
        char scoreMsg[100];
        sprintf_s(scoreMsg, "Mixed collision score: +%d points\n", scoreToAdd);
        OutputDebugStringA(scoreMsg);
    }
    
    // 체력 변화 적용 및 파괴 체크
    if (newHp1 <= 0)
    {
        shouldDestroyScript = true;
        char destroyMsg[100];
        sprintf_s(destroyMsg, "DESTROY: Type%d HP%d→%d (≤0), will be destroyed\n", 
                 static_cast<int>(type1), hp1, newHp1);
        OutputDebugStringA(destroyMsg);
        
        if (aronScene && !scoreAlreadyApplied)  // 중복 점수 방지
        {
            int destroyScore = GetDestructionScore(type1, hp1);  // 원래 체력으로 계산
            aronScene->AddScore(destroyScore);
            char scoreMsg[100];
            sprintf_s(scoreMsg, "Destruction bonus: +%d points for type %d (original HP:%d)\n", destroyScore, static_cast<int>(type1), hp1);
            OutputDebugStringA(scoreMsg);
        }
    }
    else
    {
        // 체력 업데이트 (자동 동기화)
        script->SetHealth(newHp1);
        char surviveMsg[100];
        sprintf_s(surviveMsg, "SURVIVE: Type%d HP%d→%d, health synchronized\n", 
                 static_cast<int>(type1), hp1, newHp1);
        OutputDebugStringA(surviveMsg);
    }
    
    if (newHp2 <= 0)
    {
        shouldDestroyOther = true;
        char destroyMsg2[100];
        sprintf_s(destroyMsg2, "DESTROY: Type%d HP%d→%d (≤0), will be destroyed\n", 
                 static_cast<int>(type2), hp2, newHp2);
        OutputDebugStringA(destroyMsg2);
        
        if (aronScene && !scoreAlreadyApplied)  // 중복 점수 방지
        {
            int destroyScore = GetDestructionScore(type2, hp2);  // 원래 체력으로 계산
            aronScene->AddScore(destroyScore);
            char scoreMsg[100];
            sprintf_s(scoreMsg, "Destruction bonus: +%d points for type %d (original HP:%d)\n", destroyScore, static_cast<int>(type2), hp2);
            OutputDebugStringA(scoreMsg);
        }
    }
    else
    {
        // 체력 업데이트 (자동 동기화)
        otherScript->SetHealth(newHp2);
        char surviveMsg2[100];
        sprintf_s(surviveMsg2, "SURVIVE: Type%d HP%d→%d, health synchronized\n", 
                 static_cast<int>(type2), hp2, newHp2);
        OutputDebugStringA(surviveMsg2);
    }
    
    // 물리 효과 적용
    ApplyPhysicalEffect(script, otherScript, type1, type2);
    
    // 파괴 처리
    if (shouldDestroyScript || shouldDestroyOther)
    {
        HandleDestruction(script, otherScript, shouldDestroyScript, shouldDestroyOther, aronScene);
    }
}

int HonmunCollisionTypes::GetDestructionScore(HonmunType type, int hp)
{
    // 기획서의 체력별 파괴 점수 (기획서 테이블 기준)
    switch (type)
    {
    case HonmunType::A:
        if (hp == 3) return 0;      // A(체력3) 파괴 시 0점
        else if (hp == 2) return 0; // A(체력2) 파괴 시 0점  
        else if (hp == 1) return 2; // A(체력1) 파괴 시 2점
        break;
    case HonmunType::A2: return 4;  // 2A(체력1) 파괴 시 4점
    case HonmunType::B:
        if (hp == 3) return 0;      // B(체력3) 파괴 시 0점
        else if (hp == 2) return 0; // B(체력2) 파괴 시 0점
        else if (hp == 1) return 2; // B(체력1) 파괴 시 2점 (기획서 수정)
        break;
    case HonmunType::b: return 2;   // b(체력1) 파괴 시 2점 (기획서 수정)
    case HonmunType::C:
        if (hp == 3) return 0;      // C(체력3) 파괴 시 0점
        else if (hp == 2) return 0; // C(체력2) 파괴 시 0점
        else if (hp == 1) return 2; // C(체력1) 파괴 시 2점
        break;
    case HonmunType::D: return -2;  // D(체력1) 파괴 시 -2점
    default: return 0;
    }
    return 0;
}

int HonmunCollisionTypes::GetMixedCollisionScore(HonmunType type1, int hp1, HonmunType type2, int hp2)
{
    // 기획서 A 관련 혼합 충돌 테이블 정확히 적용
    if ((type1 == HonmunType::A && hp1 == 3) || (type2 == HonmunType::A && hp2 == 3))
    {
        // A(체력3) 관련 충돌
        if ((type1 == HonmunType::B && hp1 == 3) || (type2 == HonmunType::B && hp2 == 3)) return 1;
        if ((type1 == HonmunType::B && hp1 == 2) || (type2 == HonmunType::B && hp2 == 2)) return 1;
        if ((type1 == HonmunType::B && hp1 == 1) || (type2 == HonmunType::B && hp2 == 1)) return 2;
        if ((type1 == HonmunType::b) || (type2 == HonmunType::b)) return 2;
        if ((type1 == HonmunType::C && hp1 == 3) || (type2 == HonmunType::C && hp2 == 3)) return 1;
        if ((type1 == HonmunType::C && hp1 == 2) || (type2 == HonmunType::C && hp2 == 2)) return 1;
        if ((type1 == HonmunType::C && hp1 == 1) || (type2 == HonmunType::C && hp2 == 1)) return 2;
        if ((type1 == HonmunType::D) || (type2 == HonmunType::D)) return -1;
    }
    else if ((type1 == HonmunType::A && hp1 == 2) || (type2 == HonmunType::A && hp2 == 2))
    {
        // A(체력2) 관련 충돌
        if ((type1 == HonmunType::B && hp1 == 3) || (type2 == HonmunType::B && hp2 == 3)) return 1;
        if ((type1 == HonmunType::B && hp1 == 2) || (type2 == HonmunType::B && hp2 == 2)) return 1;
        if ((type1 == HonmunType::B && hp1 == 1) || (type2 == HonmunType::B && hp2 == 1)) return 2;
        if ((type1 == HonmunType::b) || (type2 == HonmunType::b)) return 2;
        if ((type1 == HonmunType::C && hp1 == 3) || (type2 == HonmunType::C && hp2 == 3)) return 1;
        if ((type1 == HonmunType::C && hp1 == 2) || (type2 == HonmunType::C && hp2 == 2)) return 1;
        if ((type1 == HonmunType::C && hp1 == 1) || (type2 == HonmunType::C && hp2 == 1)) return 2;
        if ((type1 == HonmunType::D) || (type2 == HonmunType::D)) return -1;
    }
    else if ((type1 == HonmunType::A && hp1 == 1) || (type2 == HonmunType::A && hp2 == 1))
    {
        // A(체력1) 관련 충돌
        if ((type1 == HonmunType::B && hp1 == 3) || (type2 == HonmunType::B && hp2 == 3)) return 2;
        if ((type1 == HonmunType::B && hp1 == 2) || (type2 == HonmunType::B && hp2 == 2)) return 2;
        if ((type1 == HonmunType::B && hp1 == 1) || (type2 == HonmunType::B && hp2 == 1)) return 3;
        if ((type1 == HonmunType::b) || (type2 == HonmunType::b)) return 3;
        if ((type1 == HonmunType::C && hp1 == 3) || (type2 == HonmunType::C && hp2 == 3)) return 1;
        if ((type1 == HonmunType::C && hp1 == 2) || (type2 == HonmunType::C && hp2 == 2)) return 1;
        if ((type1 == HonmunType::C && hp1 == 1) || (type2 == HonmunType::C && hp2 == 1)) return 2;
        if ((type1 == HonmunType::D) || (type2 == HonmunType::D)) return -2;
    }
    else if ((type1 == HonmunType::A2) || (type2 == HonmunType::A2))
    {
        // 2A(체력1) 관련 충돌
        if ((type1 == HonmunType::B && hp1 == 3) || (type2 == HonmunType::B && hp2 == 3)) return 4;
        if ((type1 == HonmunType::B && hp1 == 2) || (type2 == HonmunType::B && hp2 == 2)) return 4;
        if ((type1 == HonmunType::B && hp1 == 1) || (type2 == HonmunType::B && hp2 == 1)) return 5;
        if ((type1 == HonmunType::b) || (type2 == HonmunType::b)) return 5;
        if ((type1 == HonmunType::C && hp1 == 3) || (type2 == HonmunType::C && hp2 == 3)) return 4;
        if ((type1 == HonmunType::C && hp1 == 2) || (type2 == HonmunType::C && hp2 == 2)) return 4;
        if ((type1 == HonmunType::C && hp1 == 1) || (type2 == HonmunType::C && hp2 == 1)) return 5;
        if ((type1 == HonmunType::D) || (type2 == HonmunType::D)) return -2;
    }
    else if ((type1 == HonmunType::B && hp1 == 3) || (type2 == HonmunType::B && hp2 == 3))
    {
        // B(체력3) 관련 충돌 (기획서 B 테이블 추가)
        if ((type1 == HonmunType::A && hp1 == 3) || (type2 == HonmunType::A && hp2 == 3)) return 1;
        if ((type1 == HonmunType::A && hp1 == 2) || (type2 == HonmunType::A && hp2 == 2)) return 1;
        if ((type1 == HonmunType::A && hp1 == 1) || (type2 == HonmunType::A && hp2 == 1)) return 2;
        if ((type1 == HonmunType::A2) || (type2 == HonmunType::A2)) return 4;
        if ((type1 == HonmunType::C && hp1 == 3) || (type2 == HonmunType::C && hp2 == 3)) return 1;
        if ((type1 == HonmunType::C && hp1 == 2) || (type2 == HonmunType::C && hp2 == 2)) return 1;
        if ((type1 == HonmunType::C && hp1 == 1) || (type2 == HonmunType::C && hp2 == 1)) return 2;
        if ((type1 == HonmunType::D) || (type2 == HonmunType::D)) return -1;
    }
    else if ((type1 == HonmunType::B && hp1 == 2) || (type2 == HonmunType::B && hp2 == 2))
    {
        // B(체력2) 관련 충돌
        if ((type1 == HonmunType::A && hp1 == 3) || (type2 == HonmunType::A && hp2 == 3)) return 1;
        if ((type1 == HonmunType::A && hp1 == 2) || (type2 == HonmunType::A && hp2 == 2)) return 1;
        if ((type1 == HonmunType::A && hp1 == 1) || (type2 == HonmunType::A && hp2 == 1)) return 2;
        if ((type1 == HonmunType::A2) || (type2 == HonmunType::A2)) return 4;
        if ((type1 == HonmunType::C && hp1 == 3) || (type2 == HonmunType::C && hp2 == 3)) return 1;
        if ((type1 == HonmunType::C && hp1 == 2) || (type2 == HonmunType::C && hp2 == 2)) return 1;
        if ((type1 == HonmunType::C && hp1 == 1) || (type2 == HonmunType::C && hp2 == 1)) return 2;
        if ((type1 == HonmunType::D) || (type2 == HonmunType::D)) return -1;
    }
    else if ((type1 == HonmunType::B && hp1 == 1) || (type2 == HonmunType::B && hp2 == 1))
    {
        // B(체력1) 관련 충돌
        if ((type1 == HonmunType::A && hp1 == 3) || (type2 == HonmunType::A && hp2 == 3)) return 2;
        if ((type1 == HonmunType::A && hp1 == 2) || (type2 == HonmunType::A && hp2 == 2)) return 2;
        if ((type1 == HonmunType::A && hp1 == 1) || (type2 == HonmunType::A && hp2 == 1)) return 3;
        if ((type1 == HonmunType::A2) || (type2 == HonmunType::A2)) return 5;
        if ((type1 == HonmunType::b) || (type2 == HonmunType::b)) return 2;
        if ((type1 == HonmunType::C && hp1 == 3) || (type2 == HonmunType::C && hp2 == 3)) return 2;
        if ((type1 == HonmunType::C && hp1 == 2) || (type2 == HonmunType::C && hp2 == 2)) return 2;
        if ((type1 == HonmunType::C && hp1 == 1) || (type2 == HonmunType::C && hp2 == 1)) return 3;
        if ((type1 == HonmunType::D) || (type2 == HonmunType::D)) return -2;
    }
    else if ((type1 == HonmunType::b) || (type2 == HonmunType::b))
    {
        // b(체력1) 관련 충돌 (B(체력1)과 동일한 패턴)
        if ((type1 == HonmunType::A && hp1 == 3) || (type2 == HonmunType::A && hp2 == 3)) return 2;
        if ((type1 == HonmunType::A && hp1 == 2) || (type2 == HonmunType::A && hp2 == 2)) return 2;
        if ((type1 == HonmunType::A && hp1 == 1) || (type2 == HonmunType::A && hp2 == 1)) return 3;
        if ((type1 == HonmunType::A2) || (type2 == HonmunType::A2)) return 5;
        if ((type1 == HonmunType::B && hp1 == 3) || (type2 == HonmunType::B && hp2 == 3)) return 1;
        if ((type1 == HonmunType::B && hp1 == 2) || (type2 == HonmunType::B && hp2 == 2)) return 1;
        if ((type1 == HonmunType::B && hp1 == 1) || (type2 == HonmunType::B && hp2 == 1)) return 2;
        if ((type1 == HonmunType::C && hp1 == 3) || (type2 == HonmunType::C && hp2 == 3)) return 2;
        if ((type1 == HonmunType::C && hp1 == 2) || (type2 == HonmunType::C && hp2 == 2)) return 2;
        if ((type1 == HonmunType::C && hp1 == 1) || (type2 == HonmunType::C && hp2 == 1)) return 3;
        if ((type1 == HonmunType::D) || (type2 == HonmunType::D)) return -2;
    }
    
    return 0; // 해당 없는 경우
}

void HonmunCollisionTypes::ApplyPhysicalEffect(HonmunCollisionBase* script, HonmunCollisionBase* otherScript, HonmunType type1, HonmunType type2)
{
    // A + B = 반대방향 튕김
    if ((type1 == HonmunType::A && type2 == HonmunType::B) || 
        (type1 == HonmunType::B && type2 == HonmunType::A) ||
        (type1 == HonmunType::A2 && type2 == HonmunType::B) ||
        (type1 == HonmunType::B && type2 == HonmunType::A2))
    {
        ApplyOppositeForces(script, otherScript, 500.0f);
        OutputDebugStringA("Applied opposite bounce effect\n");
    }
    // A + C = 좌우 밀림
    else if ((type1 == HonmunType::A && type2 == HonmunType::C) || 
             (type1 == HonmunType::C && type2 == HonmunType::A) ||
             (type1 == HonmunType::A2 && type2 == HonmunType::C) ||
             (type1 == HonmunType::C && type2 == HonmunType::A2))
    {
        ApplyLeftRightPush(script, otherScript, 250.0f);
        OutputDebugStringA("Applied left-right push effect\n");
    }
    // B + C = 관통
    else if ((type1 == HonmunType::B && type2 == HonmunType::C) || 
             (type1 == HonmunType::C && type2 == HonmunType::B) ||
             (type1 == HonmunType::b && type2 == HonmunType::C) ||
             (type1 == HonmunType::C && type2 == HonmunType::b))
    {
        ApplyPenetration(script, otherScript);
        OutputDebugStringA("Applied penetration effect\n");
    }
}

void HonmunCollisionTypes::HandleDMixedCollision(HonmunCollisionBase* script, HonmunCollisionBase* otherScript, Aron_Scene* aronScene)
{
    OutputDebugStringA("=== MIXED COLLISION WITH D STARTED ===\n");
    
    // Null pointer safety checks
    if (!script || !otherScript || !script->GetHonmun() || !otherScript->GetHonmun())
    {
        OutputDebugStringA("ERROR: Null pointer detected in D mixed collision, aborting\n");
        return;
    }
    
    HonmunType type1 = script->GetHonmunType();
    HonmunType type2 = otherScript->GetHonmunType();
    int hp1 = script->GetHealth();
    int hp2 = otherScript->GetHealth();
    
    // 기획서: D와의 충돌 시 -1점 상실 (HP1인 경우 -2점)
    if (aronScene)
    {
        int scoreLoss = 0;
        
        // D가 아닌 쪽의 HP에 따라 점수 상실
        if (type1 == HonmunType::D)
        {
            scoreLoss = (hp2 == 1) ? -2 : -1;
        }
        else
        {
            scoreLoss = (hp1 == 1) ? -2 : -1;
        }
        
        aronScene->AddScore(scoreLoss);
        char scoreMsg[100];
        sprintf_s(scoreMsg, "D collision penalty: %d points\n", scoreLoss);
        OutputDebugStringA(scoreMsg);
    }
    
    OutputDebugStringA("Mixed collision with D - immediate destruction!\n");
    
    // 안전한 순서로 파괴
    Honmun* honmun1 = script->GetHonmun();
    Honmun* honmun2 = otherScript->GetHonmun();
    
    if (aronScene && honmun1 && honmun2) {
        aronScene->RemoveHonmunFromManager(honmun1);
        aronScene->RemoveHonmunFromManager(honmun2);
    }
    
    if (script && !script->IsMarkedForDestroy()) {
        script->DestroyThis();
    }
    if (otherScript && !otherScript->IsMarkedForDestroy()) {
        otherScript->DestroyThis();
    }
    
    OutputDebugStringA("=== MIXED COLLISION WITH D COMPLETED ===\n");
}

void HonmunCollisionTypes::HandleDestruction(HonmunCollisionBase* script, HonmunCollisionBase* otherScript, 
                                           bool destroyScript, bool destroyOther, Aron_Scene* aronScene)
{
    OutputDebugStringA("=== HandleDestruction STARTED ===\n");
    
    try {
        if (destroyScript && script && !script->IsMarkedForDestroy())
        {
            OutputDebugStringA("Safely destroying first script object...\n");
            
            // 1. 관리자에서 제거
            if (aronScene && script->GetHonmun()) {
                aronScene->RemoveHonmunFromManager(script->GetHonmun());
            }
            
            // 2. 충돌 관리자에서 제거
            if (script->GetCollisionManager()) {
                script->GetCollisionManager()->UnregisterHonmun(script);
            }
            
            // 3. 파괴 마크
            script->DestroyThis();
            OutputDebugStringA("First script object marked for destruction\n");
        }
        
        if (destroyOther && otherScript && !otherScript->IsMarkedForDestroy())
        {
            OutputDebugStringA("Safely destroying second script object...\n");
            
            // 1. 관리자에서 제거
            if (aronScene && otherScript->GetHonmun()) {
                aronScene->RemoveHonmunFromManager(otherScript->GetHonmun());
            }
            
            // 2. 충돌 관리자에서 제거
            if (otherScript->GetCollisionManager()) {
                otherScript->GetCollisionManager()->UnregisterHonmun(otherScript);
            }
            
            // 3. 파괴 마크
            otherScript->DestroyThis();
            OutputDebugStringA("Second script object marked for destruction\n");
        }
        
    } catch (...) {
        OutputDebugStringA("EXCEPTION in HandleDestruction: Continuing safely...\n");
    }
    
    OutputDebugStringA("=== HandleDestruction COMPLETED ===\n");
}

bool HonmunCollisionTypes::ShouldProcessCollision(HonmunCollisionBase* script1, HonmunCollisionBase* script2)
{
    // Check for null pointers
    if (!script1 || !script2)
    {
        OutputDebugStringA("ShouldProcessCollision: One or both scripts are null\n");
        return false;
    }
    
    // Check if objects are already marked for destruction
    if (script1->IsMarkedForDestroy() || script2->IsMarkedForDestroy())
    {
        OutputDebugStringA("ShouldProcessCollision: One or both objects already marked for destruction\n");
        return false;
    }
    
    // Check if Honmun objects are valid
    if (!script1->GetHonmun() || !script2->GetHonmun())
    {
        OutputDebugStringA("ShouldProcessCollision: One or both Honmun objects are null\n");
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
    // A+B 충돌: 서로 반대 방향으로 자연스럽게 튕겨냄
    auto* transform1 = script1->GetTransform();
    auto* transform2 = script2->GetTransform();
    auto* rigidbody1 = script1->GetRigidbody();
    auto* rigidbody2 = script2->GetRigidbody();
    
    if (!transform1 || !transform2 || !rigidbody1 || !rigidbody2) return;
    
    Vector2 pos1 = transform1->GetPosition();
    Vector2 pos2 = transform2->GetPosition();
    
    // 충돌 방향 벡터 계산
    Vector2 direction = (pos1 - pos2).Normalized();
    
    // 반대 방향으로 속도 적용 (자연스러운 튕김)
    Vector2 bounce1 = direction * force;        // script1은 멀어지는 방향
    Vector2 bounce2 = -direction * force;       // script2는 반대 방향
    
    // 위쪽으로 약간 띄우기 (더 역동적인 효과)
    bounce1.y += force * 0.3f;
    bounce2.y += force * 0.3f;
    
    // rigidbody에 속도 적용
    rigidbody1->velocity = bounce1;
    rigidbody2->velocity = bounce2;
    
    char debugMsg[150];
    sprintf_s(debugMsg, "A+B Bounce: obj1(%.1f,%.1f) obj2(%.1f,%.1f)\n", 
             bounce1.x, bounce1.y, bounce2.x, bounce2.y);
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