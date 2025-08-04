#include "HonmunCollisionBase.h"
#include "HonmunCollisionTypes.h"
#include "HonmunCollisionEffects.h"
#include "HonmunCollisionManager.h"
#include "PlayerFSM.h"
#include "Aron_Scene.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/SceneManager.h"
#include "../Direct2D_EngineLib/Input.h"
#include "../Direct2D_EngineLib/Camera.h"

HonmunCollisionBase::HonmunCollisionBase()
{
    InitializeHelperClasses();
}

HonmunCollisionBase::~HonmunCollisionBase()
{
    CleanupHelperClasses();
}

void HonmunCollisionBase::Awake()
{
    // 안전한 gameObject 접근 체크
    if (!gameObject) {
        OutputDebugStringA("CRITICAL ERROR: gameObject is null in HonmunCollisionBase::Awake\n");
        return;
    }
    
    try {
        // Get component references with safety checks
        honmun = dynamic_cast<Honmun*>(gameObject);
        transform = gameObject->GetComponent<Transform>();
        rigidbody = gameObject->GetComponent<Rigidbody>();
        spriteRenderer = gameObject->GetComponent<SpriteRenderer>();
        
        // Initialize properties with safety checks
        if (honmun)
        {
            honmunType = honmun->GetHonmunType();
            health = honmun->GetHP();
            currentSize = honmun->GetSize();
            
            // 체력 동기화 확인 로그
            char syncMsg[100];
            sprintf_s(syncMsg, "Health sync at Awake: Honmun HP=%d, Script health=%d\n", 
                     honmun->GetHP(), health);
            OutputDebugStringA(syncMsg);
            
            // 안전한 이름 접근 with 디버그 로그
            std::string safeName = "UNKNOWN";
            try {
                if (!gameObject->name.empty()) {
                    safeName = gameObject->name;
                }
            } catch (...) {
                OutputDebugStringA("WARNING: Exception accessing gameObject->name in Awake\n");
                safeName = "CORRUPTED_NAME";
            }
            
            char debugMsg[150];
            sprintf_s(debugMsg, "HonmunCollisionBase Awake: Type %d, Name: %s\n", 
                     static_cast<int>(honmunType), safeName.c_str());
            OutputDebugStringA(debugMsg);
        } else {
            OutputDebugStringA("ERROR: Failed to cast gameObject to Honmun in Awake\n");
        }
        
    } catch (...) {
        OutputDebugStringA("EXCEPTION: Critical error in HonmunCollisionBase::Awake\n");
        // 기본값으로 초기화
        honmunType = HonmunType::A;
        health = 3;
        currentSize = 1.0f;
    }
}

void HonmunCollisionBase::Start()
{
    // Additional initialization after all components are set up
    // 콜라이더 등록 상태 확인
    auto* collider = gameObject->GetComponent<CircleCollider>();
    if (collider)
    {
        char debugMsg[150];
        sprintf_s(debugMsg, "Honmun %s: CircleCollider found, isTrigger=%s, radius=%.1f\n", 
                 gameObject->name.c_str(), 
                 collider->isTrigger ? "true" : "false",
                 collider->radius);
        OutputDebugStringA(debugMsg);
    }
    else
    {
        OutputDebugStringA("ERROR: No CircleCollider found!\n");
    }
}

void HonmunCollisionBase::Update()
{
    // Handle reaction cooldown
    if (reactionCooldown > 0.0f)
    {
        reactionCooldown -= Time::GetDeltaTime();
        if (reactionCooldown <= 0.0f)
        {
            isProcessingReaction = false;
        }
    }
    
    // 키네마틱 모드에서 수동 낙하 시스템 (A+C 충돌 후 정지 상태가 아닐 때만)
    if (rigidbody && transform && !markedForDestroy && !isPushedAndStopped)
    {
        // 키네마틱 모드 유지하되 충돌 감지를 위해 특별 설정
        rigidbody->isKinematic = false; // 트리거 충돌 감지를 위해 키네마틱 해제
        rigidbody->useGravity = false;  // 중력은 비활성화 유지
        rigidbody->drag = 10.0f;        // 높은 드래그로 물리 움직임 최소화
        
        // 타입별 낙하속도로 수동 낙하 (기획서: A,B,C(1) D(2))
        float fallingSpeed = GetFallingSpeedByType();
        Vector2 currentPos = transform->GetPosition();
        
        // 매 프레임 아래로 이동
        float deltaTime = Time::GetDeltaTime();
        float previousY = currentPos.y;
        currentPos.y -= fallingSpeed * 50.0f * deltaTime; // 50.0f는 속도 배율
        
        // Y좌표 변화 추적 (5프레임마다 한번씩만 로그)
        static int fallLogCounter = 0;
        fallLogCounter++;
        if (fallLogCounter % 5 == 0 && abs(previousY - currentPos.y) > 0.1f)
        {
            char fallMsg[200];
            sprintf_s(fallMsg, "혼문 낙하: Type%d, Y좌표 %.1f→%.1f (속도=%.1f, Ground=-350)\n", 
                     static_cast<int>(honmunType), previousY, currentPos.y, fallingSpeed);
            OutputDebugStringA(fallMsg);
            
            // 물리 상태 추가 로그
            sprintf_s(fallMsg, "물리상태: isKinematic=%s, useGravity=%s, drag=%.1f\n", 
                     rigidbody->isKinematic ? "true" : "false",
                     rigidbody->useGravity ? "true" : "false",
                     rigidbody->drag);
            OutputDebugStringA(fallMsg);
            
            // Ground에 근접했을 때 특별히 로그
            if (currentPos.y < -320.0f && currentPos.y > -380.0f)
            {
                sprintf_s(fallMsg, "⚠️ Ground 근접: Y=%.1f (Ground=-350, 차이=%.1f)\n", 
                         currentPos.y, currentPos.y - (-350.0f));
                OutputDebugStringA(fallMsg);
                
                // 콜라이더 상태도 확인
                auto* collider = gameObject->GetComponent<CircleCollider>();
                if (collider) {
                    sprintf_s(fallMsg, "콜라이더 상태: isTrigger=%s, radius=%.1f, offset=(%.1f,%.1f)\n", 
                             collider->isTrigger ? "true" : "false",
                             collider->radius, collider->offset.x, collider->offset.y);
                    OutputDebugStringA(fallMsg);
                }
            }
        }
        
        transform->SetPosition(currentPos.x, currentPos.y);
    }
    
    // 플레이어 공격 넉백 처리 (속도 5)
    if (isBeingKnockedBack && knockbackRemainingTime > 0.0f)
    {
        float deltaTime = Time::GetDeltaTime();
        knockbackRemainingTime -= deltaTime;
        
        // 속도 10으로 넉백 방향으로 이동
        Vector2 currentPos = transform->GetPosition();
        float knockbackSpeed = 10.0f;
        currentPos += knockbackDirection * knockbackSpeed * deltaTime * 50.0f; // 50.0f는 속도 배율
        
        transform->SetPosition(currentPos.x, currentPos.y);
        
        // 넉백 시간 종료
        if (knockbackRemainingTime <= 0.0f)
        {
            isBeingKnockedBack = false;
        }
    }
    
    // Handle physics transition for split fragments - 포켓볼 스타일 퍼짐 후 정착
    if (needsPhysicsTransition)
    {
        splitPhysicsTimer += Time::GetDeltaTime();
        
        // 연쇄 충돌 최적화: 빠른 정착으로 연쇄 충돌 촉진
        if (rigidbody && splitPhysicsTimer > 0.2f) // 0.2초 후부터 감속 시작 (연쇄 충돌용)
        {
            Vector2 currentVel = rigidbody->velocity;
            float decelerationRate = 0.85f; // 매 프레임 15% 감속 (더 빠른 정착)
            rigidbody->velocity = currentVel * decelerationRate;
            
            // 속도가 충분히 느려지면 정착 (연쇄 충돌을 위해 빠르게)
            if (currentVel.Magnitude() < 30.0f) // 임계값 증가 (더 빠른 정착)
            {
                rigidbody->useGravity = false;   // 중력 비활성화 유지
                rigidbody->isKinematic = true;   // 키네마틱 모드로 전환 (충돌 감지 가능)
                rigidbody->velocity = Vector2(0, 0); // 속도 완전 정지
                
                OutputDebugStringA("Fragment settled quickly for CHAIN COLLISION\n");
                needsPhysicsTransition = false;
                splitPhysicsTimer = 0.0f;
            }
        }
        
        // 최대 1초 후 강제 정착 (연쇄 충돌을 위해 빠르게)
        if (splitPhysicsTimer >= 1.0f)
        {
            if (rigidbody)
            {
                rigidbody->useGravity = false;
                rigidbody->isKinematic = true;
                rigidbody->velocity = Vector2(0, 0);
                OutputDebugStringA("Fragment force-settled after 1 second for CHAIN COLLISION\n");
            }
            needsPhysicsTransition = false;
            splitPhysicsTimer = 0.0f;
        }
    }
    
    // 타입별 마찰력 적용 및 b 조각 지속 운동 (성능 최적화)
    if (!markedForDestroy)
    {
        ApplyTypeSpecificFriction();
        
        // b 조각들은 지속적인 운동량 유지
        if (honmunType == HonmunType::b && isSplitFragment)
        {
            MaintainFragmentMomentum();
        }
    }
}

void HonmunCollisionBase::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
{
    // 모든 트리거 진입 로그 (디버깅용)
    if (other && other->gameObject) {
        char triggerMsg[200];
        sprintf_s(triggerMsg, "🔥 OnTriggerEnter: %s 와 %s 충돌!\n", 
                 gameObject ? gameObject->name.c_str() : "NULL", 
                 other->gameObject->name.c_str());
        OutputDebugStringA(triggerMsg);
    }
    
    // Early safety checks before any processing
    if (markedForDestroy)
    {
        return; // 빠른 종료 - 디버그 메시지 제거로 성능 향상
    }
    
    // 필수 충돌만 로그 (성능 최적화)
    if (other->gameObject && other->gameObject->name == "PlayerAttackArea")
    {
        OutputDebugStringA("*** PLAYER ATTACK HIT ***\n");
    }
    
    if (!collisionManager || !other || !other->gameObject) 
    {
        return; // 빠른 종료 - 성능 최적화
    }
    
    auto* otherHonmun = dynamic_cast<Honmun*>(other->gameObject);
    if (!otherHonmun) 
    {
        
        // 플레이어와 충돌 시 관통 + 이동속도 20% 감소 2초간
        if (other->gameObject && other->gameObject->tag == "Player") 
        {
            // 플레이어에게 속도 디버프 적용 (최적화된 버전)
            auto scripts = other->gameObject->GetComponents<Script>();
            for (auto* script : scripts) 
            {
                if (auto* playerFSM = dynamic_cast<PlayerFSM*>(script)) 
                {
                    playerFSM->SetSpeedDownRate(0.8f); // 20% 감소
                    break;
                }
            }
        }
        // 플레이어 공격 영역과의 충돌 감지 (최적화)
        else if (other->gameObject && other->gameObject->name == "PlayerAttackArea")
        {
            if (rigidbody && transform)
            {
                // 플레이어 공격 시 마우스 방향 기반 넉백
                Vector2 honmunPos = transform->GetPosition();
                Vector2 attackPos = other->gameObject->GetComponent<Transform>()->GetPosition();
                
                // 마우스 월드 좌표 계산
                Vector2 mouseWorldPos = Camera::GetScreenToWorldPosition(Input::GetMouseScreenPosition());
                
                // 플레이어 위치 찾기 (Aron_Scene에서 Player 가져오기)
                Vector2 playerPos = attackPos; // 기본값
                
                auto* currentScene = SceneManager::Get().GetCurrentScene();
                auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
                if (aronScene && aronScene->GetPlayer()) {
                    auto* playerTransform = aronScene->GetPlayer()->GetComponent<Transform>();
                    if (playerTransform) {
                        playerPos = playerTransform->GetPosition();
                    }
                }
                
                // 넉백 방향 = 플레이어에서 마우스로의 방향
                Vector2 knockbackDir = (mouseWorldPos - playerPos).Normalized();
                
                // 디버그: 방향 계산 로그
                char dirDebugMsg[200];
                sprintf_s(dirDebugMsg, "🎯 Knockback Direction: Player(%.1f,%.1f) → Mouse(%.1f,%.1f) = Dir(%.2f,%.2f)\n", 
                         playerPos.x, playerPos.y, mouseWorldPos.x, mouseWorldPos.y, knockbackDir.x, knockbackDir.y);
                OutputDebugStringA(dirDebugMsg);
                
                // 넉백 속도를 10으로 설정
                float knockbackSpeed = 10.0f; // 속도 10으로 수정
                Vector2 knockbackVelocity = knockbackDir * knockbackSpeed;
                
                // 키네마틱 모드에서 velocity 대신 수동으로 이동 처리 필요
                // (매 프레임 Update에서 처리하도록 knockback 상태 설정)
                isBeingKnockedBack = true;
                knockbackDirection = knockbackDir;
                knockbackRemainingTime = 2.0f; // 2초간 밀려남
            }
        }
        // 가장 긴 콜라이더(실제 바닥)와 충돌 시에만 혼문 제거 + 혼의 개수 감소
        else if (other->gameObject && other->gameObject->name == "Ground")
        {
            // 상세한 Ground 충돌 분석 로그
            Vector2 honmunPos = transform ? transform->GetPosition() : Vector2(0, 0);
            Vector2 groundPos = other->gameObject->GetComponent<Transform>() ? 
                               other->gameObject->GetComponent<Transform>()->GetPosition() : Vector2(0, 0);
            
            char groundDebugMsg[200];
            sprintf_s(groundDebugMsg, "🚨 *** GROUND COLLISION DETECTED *** 🚨\n");
            OutputDebugStringA(groundDebugMsg);
            
            sprintf_s(groundDebugMsg, "혼문 중심: (%.1f, %.1f), Ground 중심: (%.1f, %.1f)\n", 
                     honmunPos.x, honmunPos.y, groundPos.x, groundPos.y);
            OutputDebugStringA(groundDebugMsg);
            
            // 혼문 콜라이더 정보 추가
            auto* honmunCollider = gameObject->GetComponent<CircleCollider>();
            if (honmunCollider) {
                float honmunBottom = honmunPos.y + honmunCollider->offset.y + honmunCollider->radius;
                sprintf_s(groundDebugMsg, "혼문 콜라이더: 반지름=%.1f, 오프셋Y=%.1f, 아래끝=%.1f\n", 
                         honmunCollider->radius, honmunCollider->offset.y, honmunBottom);
                OutputDebugStringA(groundDebugMsg);
            }
            
            auto* boxCollider = other->gameObject->GetComponent<BoxCollider>();
            if (boxCollider)
            {
                sprintf_s(groundDebugMsg, "Ground BoxCollider 크기: %.1f x %.1f, 오프셋: (%.1f, %.1f)\n", 
                         boxCollider->size.x, boxCollider->size.y, 
                         boxCollider->offset.x, boxCollider->offset.y);
                OutputDebugStringA(groundDebugMsg);
                
                // 실제 충돌 영역 계산
                float groundTop = groundPos.y + boxCollider->offset.y - (boxCollider->size.y * 0.5f);
                float groundBottom = groundPos.y + boxCollider->offset.y + (boxCollider->size.y * 0.5f);
                
                sprintf_s(groundDebugMsg, "Ground 박스 충돌영역: Top=%.1f, Bottom=%.1f, Center=%.1f\n", 
                         groundTop, groundBottom, groundPos.y + boxCollider->offset.y);
                OutputDebugStringA(groundDebugMsg);
                
                // 혼문과 Ground 사이의 거리 계산
                auto* honmunCollider = gameObject->GetComponent<CircleCollider>();
                if (honmunCollider) {
                    float honmunBottom = honmunPos.y + honmunCollider->offset.y + honmunCollider->radius;
                    float distanceToGround = honmunBottom - groundTop;
                    sprintf_s(groundDebugMsg, "🎯 충돌 분석: 혼문아래끝=%.1f, Ground위끝=%.1f, 침투깊이=%.1f\n", 
                             honmunBottom, groundTop, distanceToGround);
                    OutputDebugStringA(groundDebugMsg);
                }
                
                // Ground 객체 식별
                std::string groundType = "UNKNOWN";
                if (abs(groundPos.y - (-350.0f)) < 10.0f && boxCollider->size.x > 1500.0f) {
                    groundType = "메인바닥";
                } else if (abs(groundPos.y - (-200.0f)) < 10.0f && boxCollider->size.x < 300.0f) {
                    groundType = "플랫폼1";
                } else if (abs(groundPos.y - 0.0f) < 10.0f && boxCollider->size.x < 300.0f) {
                    groundType = "플랫폼2";
                } else if (abs(groundPos.y - (-350.0f)) < 10.0f && boxCollider->size.x < 1200.0f) {
                    groundType = "플레이어바닥";
                }
                
                sprintf_s(groundDebugMsg, "🏗️ Ground 객체 식별: %s (Y=%.1f, 크기=%.1fx%.1f)\n", 
                         groundType.c_str(), groundPos.y, boxCollider->size.x, boxCollider->size.y);
                OutputDebugStringA(groundDebugMsg);
                
                // 높은 위치에 있는 혼문은 아직 땅에 도달하지 않았으므로 파괴하지 않음
                if (honmunPos.y > -345.0f) // 바닥(-350) 위쪽 5픽셀 여유를 둠
                {
                    sprintf_s(groundDebugMsg, "🔄 혼문이 아직 높은 위치 (Y=%.1f > -345), 관통 처리\n", 
                             honmunPos.y);
                    OutputDebugStringA(groundDebugMsg);
                }
                else if (boxCollider->size.x > 1500.0f) // 진짜 바닥만
                {
                    sprintf_s(groundDebugMsg, "✅ 진짜 바닥 확인됨 (%s, 크기=%.1f > 1500), 혼문 파괴!\n", 
                             groundType.c_str(), boxCollider->size.x);
                    OutputDebugStringA(groundDebugMsg);
                    
                    // 아론 씬에서 혼의 개수 감소
                    auto* currentScene = SceneManager::Get().GetCurrentScene();
                    auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
                    if (aronScene)
                    {
                        aronScene->DecreaseSoulCount();
                    }
                    DestroyThis();
                }
                else
                {
                    sprintf_s(groundDebugMsg, "🔄 플랫폼/짧은콜라이더 (%s, 크기=%.1f ≤ 1500), 관통 처리\n", 
                             groundType.c_str(), boxCollider->size.x);
                    OutputDebugStringA(groundDebugMsg);
                }
            }
            else
            {
                OutputDebugStringA("ERROR: Ground 객체에 BoxCollider가 없음!\n");
            }
        }
        return;
    }
    
    HonmunCollisionBase* otherScript = otherHonmun->GetComponent<HonmunCollisionBase>();
    if (!otherScript || otherScript->IsMarkedForDestroy() || !honmun || !otherHonmun) 
    {
        return; // 빠른 안전성 체크
    }
    
    // 연쇄 반응 허용 - 짧은 쿨타임으로 같은 쌍 충돌만 방지
    if (reactionCooldown > 0.0f || otherScript->reactionCooldown > 0.0f) 
    {
        return;
    }
    
    // 충돌 처리 플래그 설정
    isProcessingReaction = true;
    otherScript->SetProcessingReaction(true);
    
    // 충돌 타입 핸들러로 위임
    if (collisionTypes)
    {
        collisionTypes->ProcessCollision(this, otherScript, contact);
    }
    
    // 쿨타임 설정 (객체가 여전히 유효한 경우만)
    if (!markedForDestroy && !otherScript->IsMarkedForDestroy())
    {
        reactionCooldown = 0.03f;  // 더 빠른 연쇄 반응
        otherScript->reactionCooldown = 0.03f;
    }
}

void HonmunCollisionBase::OnCollisionEnter(ICollider* other, const ContactInfo& contact)
{
    // 박스 콜라이더와의 충돌 테스트용
    char debugMsg[100];
    sprintf_s(debugMsg, "💥 OnCollisionEnter called! Other: %s\n", 
              other->gameObject ? other->gameObject->name.c_str() : "null");
    OutputDebugStringA(debugMsg);
    
    // Ground 충돌도 OnCollisionEnter에서 처리 (playerGround는 isTrigger=false)
    if (other && other->gameObject && other->gameObject->name == "Ground")
    {
        OutputDebugStringA("💥 OnCollisionEnter - Ground 충돌 감지됨!\n");
        
        // 상세한 Ground 충돌 분석 로그 (OnTriggerEnter와 동일)
        Vector2 honmunPos = transform ? transform->GetPosition() : Vector2(0, 0);
        Vector2 groundPos = other->gameObject->GetComponent<Transform>() ? 
                           other->gameObject->GetComponent<Transform>()->GetPosition() : Vector2(0, 0);
        
        char groundDebugMsg[200];
        sprintf_s(groundDebugMsg, "🚨 *** OnCollisionEnter GROUND COLLISION *** 🚨\n");
        OutputDebugStringA(groundDebugMsg);
        
        sprintf_s(groundDebugMsg, "혼문 중심: (%.1f, %.1f), Ground 중심: (%.1f, %.1f)\n", 
                 honmunPos.x, honmunPos.y, groundPos.x, groundPos.y);
        OutputDebugStringA(groundDebugMsg);
        
        auto* boxCollider = other->gameObject->GetComponent<BoxCollider>();
        if (boxCollider)
        {
            sprintf_s(groundDebugMsg, "Ground BoxCollider 크기: %.1f x %.1f, isTrigger=%s\n", 
                     boxCollider->size.x, boxCollider->size.y, 
                     boxCollider->isTrigger ? "true" : "false");
            OutputDebugStringA(groundDebugMsg);
            
            // Ground 객체 식별
            std::string groundType = "UNKNOWN";
            if (abs(groundPos.y - (-350.0f)) < 10.0f && boxCollider->size.x > 1500.0f) {
                groundType = "메인바닥";
            } else if (abs(groundPos.y - (-350.0f)) < 10.0f && boxCollider->size.x < 1200.0f) {
                groundType = "플레이어바닥";
            } else if (abs(groundPos.y - (-200.0f)) < 10.0f && boxCollider->size.x < 300.0f) {
                groundType = "플랫폼1";
            } else if (abs(groundPos.y - 0.0f) < 10.0f && boxCollider->size.x < 300.0f) {
                groundType = "플랫폼2";
            }
            
            sprintf_s(groundDebugMsg, "🏗️ OnCollisionEnter Ground 식별: %s (Y=%.1f, 크기=%.1fx%.1f)\n", 
                     groundType.c_str(), groundPos.y, boxCollider->size.x, boxCollider->size.y);
            OutputDebugStringA(groundDebugMsg);
            
            // 높은 위치에 있는 혼문은 아직 땅에 도달하지 않았으므로 파괴하지 않음
            if (honmunPos.y > -345.0f) // 바닥(-350) 위쪽 5픽셀 여유를 둠
            {
                sprintf_s(groundDebugMsg, "🔄 OnCollisionEnter - 혼문이 아직 높은 위치 (Y=%.1f > -345), 관통 처리\n", 
                         honmunPos.y);
                OutputDebugStringA(groundDebugMsg);
            }
            // 메인바닥에서만 파괴 (OnTriggerEnter와 동일 로직)
            else if (boxCollider->size.x > 1500.0f) 
            {
                sprintf_s(groundDebugMsg, "✅ OnCollisionEnter - 진짜 바닥 확인됨 (%s), 혼문 파괴!\n", 
                         groundType.c_str());
                OutputDebugStringA(groundDebugMsg);
                
                auto* currentScene = SceneManager::Get().GetCurrentScene();
                auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
                if (aronScene)
                {
                    aronScene->DecreaseSoulCount();
                }
                DestroyThis();
            }
            else
            {
                sprintf_s(groundDebugMsg, "🔄 OnCollisionEnter - 플랫폼/플레이어바닥 (%s), 물리 충돌만\n", 
                         groundType.c_str());
                OutputDebugStringA(groundDebugMsg);
            }
        }
    }
}

void HonmunCollisionBase::SetHonmunType(HonmunType type)
{
    honmunType = type;
    if (honmun)
    {
        honmun->SetHonmunType(type);
    }
}

void HonmunCollisionBase::DestroyThis()
{
    if (markedForDestroy) return; // 이미 파괴 표시됨
    
    markedForDestroy = true;
    
    char debugMsg[100];
    sprintf_s(debugMsg, "DestroyThis called for %s - MARKED for destruction\n", 
              gameObject ? gameObject->name.c_str() : "null");
    OutputDebugStringA(debugMsg);
    
    //  FIXED: 즉시 파괴하지 말고 비활성화만 하기
    if (honmun && honmun->IsActive())
    {
        honmun->SetActive(false);  // 즉시 비활성화
        OutputDebugStringA("Object deactivated safely\n");
    }
    
    //  FIXED: 실제 파괴는 Scene의 Update 끝에서 안전하게 처리
    // honmun->Destroy(); // 제거: 즉시 파괴 금지
}

void HonmunCollisionBase::InitializeHelperClasses()
{
    collisionTypes = new HonmunCollisionTypes();
    collisionEffects = new HonmunCollisionEffects();
    collisionManager = new HonmunCollisionManager();
}

void HonmunCollisionBase::CleanupHelperClasses()
{
    delete collisionTypes;
    delete collisionEffects;
    delete collisionManager;
    
    collisionTypes = nullptr;
    collisionEffects = nullptr;
    collisionManager = nullptr;
}

void HonmunCollisionBase::ApplyTypeSpecificFriction()
{
    if (currentVelocity.Magnitude() > minVelocity)
    {
        float typeFriction = GetFrictionByType(honmunType);
        currentVelocity *= typeFriction;
        
        // Rigidbody 속도도 함께 감소시킴
        if (rigidbody && !needsPhysicsTransition)
        {
            Vector2 rbVelocity = rigidbody->velocity;
            if (rbVelocity.Magnitude() > minVelocity)
            {
                rigidbody->velocity = rbVelocity * typeFriction;
            }
        }
    }
    else
    {
        currentVelocity = Vector2(0, 0);
    }
}

void HonmunCollisionBase::MaintainFragmentMomentum()
{
    if (!rigidbody) return;
    
    // b 조각들은 분열 시 받은 초기 속도를 persistentVelocity에 저장
    if (persistentVelocity.Magnitude() > 5.0f)  // 최소 임계값
    {
        // 지속적인 운동량 감소 (천천히)
        persistentVelocity *= fragmentMomentumDecay;
        
        // Rigidbody에 지속 속도 적용
        Vector2 currentRbVel = rigidbody->velocity;
        
        // 현재 속도가 너무 작으면 지속 속도로 보충
        if (currentRbVel.Magnitude() < persistentVelocity.Magnitude() * 0.5f)
        {
            rigidbody->velocity = persistentVelocity;
            
            char momentumMsg[100];
            sprintf_s(momentumMsg, "b fragment momentum maintained: velocity(%.1f, %.1f)\n", 
                     persistentVelocity.x, persistentVelocity.y);
            OutputDebugStringA(momentumMsg);
        }
    }
}

float HonmunCollisionBase::GetFrictionByType(HonmunType type)
{
    // 타입별 차등 마찰력 (값이 작을수록 빨리 멈춤)
    switch (type)
    {
    case HonmunType::A:  return 0.94f;  // A: 높은 마찰력 (빨리 멈춤)
    case HonmunType::A2: return 0.92f;  // 2A: 더 높은 마찰력 (무거워서 빨리 멈춤)
    case HonmunType::B:  return 0.96f;  // B: 중간 마찰력
    case HonmunType::b:  return 0.985f; // b: 낮은 마찰력 (오래 움직임)
    case HonmunType::C:  return 0.93f;  // C: 높은 마찰력
    case HonmunType::D:  return 0.91f;  // D: 가장 높은 마찰력 (빨리 멈춤)
    default: return 0.95f;  // 기본값
    }
}

float HonmunCollisionBase::GetFallingSpeedByType()
{
    // 키네마틱 모드용 낙하속도 (기획서: A,B,C(1) D(2))
    switch (honmunType)
    {
    case HonmunType::A:
    case HonmunType::B:
    case HonmunType::C:
    case HonmunType::A2:
    case HonmunType::b:
        return 1.0f; // A, B, C 타입: 속도 1
    case HonmunType::D:
        return 2.0f; // D 타입: 속도 2
    default:
        return 1.0f; // 기본값
    }
}