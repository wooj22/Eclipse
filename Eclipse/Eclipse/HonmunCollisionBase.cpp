#include "HonmunCollisionBase.h"
#include "HonmunCollisionTypes.h"
#include "HonmunCollisionEffects.h"
#include "HonmunCollisionManager.h"
#include "PlayerFSM.h"
#include "Aron_Scene.h"
#include "../Direct2D_EngineLib/Time.h"
#include "../Direct2D_EngineLib/SceneManager.h"

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
    
    // 키네마틱 모드에서 수동 낙하 시스템
    if (rigidbody && transform && !markedForDestroy)
    {
        // 키네마틱 모드 유지 (중력 없이 동작)
        rigidbody->isKinematic = true;
        rigidbody->useGravity = false;
        
        // 타입별 낙하속도로 수동 낙하 (기획서: A,B,C(1) D(2))
        float fallingSpeed = GetFallingSpeedByType();
        Vector2 currentPos = transform->GetPosition();
        
        // 매 프레임 아래로 이동
        float deltaTime = Time::GetDeltaTime();
        currentPos.y -= fallingSpeed * 50.0f * deltaTime; // 50.0f는 속도 배율
        
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
                // 플레이어 공격 시 속도 5로 천천히 밀려나도록 설정
                Vector2 honmunPos = transform->GetPosition();
                Vector2 attackPos = other->gameObject->GetComponent<Transform>()->GetPosition();
                
                // 공격 방향 = 혼문 위치 - 공격 위치 (플레이어가 공격한 방향)
                Vector2 knockbackDir = (honmunPos - attackPos).Normalized();
                
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
            auto* boxCollider = other->gameObject->GetComponent<BoxCollider>();
            if (boxCollider && boxCollider->size.x > 1500.0f) // 진짜 바닥만
            {
                // 아론 씬에서 혼의 개수 감소
                auto* currentScene = SceneManager::Get().GetCurrentScene();
                auto* aronScene = dynamic_cast<Aron_Scene*>(currentScene);
                if (aronScene)
                {
                    aronScene->DecreaseSoulCount();
                }
                DestroyThis();
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
    sprintf_s(debugMsg, "OnCollisionEnter called! Other: %s\n", 
              other->gameObject ? other->gameObject->name.c_str() : "null");
    OutputDebugStringA(debugMsg);
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