#include "HonmunCollisionBase.h"
#include "HonmunCollisionTypes.h"
#include "HonmunCollisionEffects.h"
#include "HonmunCollisionManager.h"
#include "../Direct2D_EngineLib/Time.h"

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
    
    // 타입별 마찰력 적용 및 b 조각 지속 운동
    ApplyTypeSpecificFriction();
    
    // b 조각들은 지속적인 운동량 유지
    if (honmunType == HonmunType::b && isSplitFragment)
    {
        MaintainFragmentMomentum();
    }
}

void HonmunCollisionBase::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
{
    // Early safety checks before any processing
    if (markedForDestroy)
    {
        OutputDebugStringA("OnTriggerEnter: This object is marked for destruction, ignoring collision\n");
        return;
    }
    
    // 모든 충돌 시도를 디버그로 기록
    char allDebugMsg[200];
    sprintf_s(allDebugMsg, "*** TRIGGER DETECTED *** %s collides with %s\n", 
              gameObject ? gameObject->name.c_str() : "null",
              other->gameObject ? other->gameObject->name.c_str() : "null");
    OutputDebugStringA(allDebugMsg);
    
    if (!collisionManager) 
    {
        OutputDebugStringA("No collision manager!\n");
        return;
    }
    
    // Get other collision script with additional safety checks
    if (!other || !other->gameObject)
    {
        OutputDebugStringA("Other collider or gameObject is null!\n");
        return;
    }
    
    auto* otherHonmun = dynamic_cast<Honmun*>(other->gameObject);
    if (!otherHonmun) 
    {
        OutputDebugStringA("Other object is not Honmun!\n");
        return;
    }
    
    HonmunCollisionBase* otherScript = otherHonmun->GetComponent<HonmunCollisionBase>();
    if (!otherScript) 
    {
        OutputDebugStringA("Other Honmun has no collision script!\n");
        return;
    }
    
    // Check if other object is marked for destruction
    if (otherScript->IsMarkedForDestroy())
    {
        OutputDebugStringA("Other object is marked for destruction, ignoring collision\n");
        return;
    }
    
    // Allow chain reactions - only prevent same pair collision within short time
    if (reactionCooldown > 0.0f || otherScript->reactionCooldown > 0.0f) 
    {
        OutputDebugStringA("Collision cooldown active, skipping\n");
        return;
    }
    
    // Additional component validity checks
    if (!honmun || !otherHonmun)
    {
        OutputDebugStringA("One or both Honmun objects are null, aborting collision\n");
        return;
    }
    
    // 키네마틱 모드에서 충돌 처리 디버그 메시지
    char debugMsg[100];
    sprintf_s(debugMsg, "SUCCESS! Honmun collision: Type %d vs Type %d\n", 
              static_cast<int>(honmunType), static_cast<int>(otherScript->GetHonmunType()));
    OutputDebugStringA(debugMsg);
    
    // Set processing flag BEFORE calling ProcessCollision (but after all checks)
    isProcessingReaction = true;
    otherScript->SetProcessingReaction(true);
    
    // Delegate to collision types handler
    if (collisionTypes)
    {
        OutputDebugStringA("Calling collisionTypes->ProcessCollision...\n");
        collisionTypes->ProcessCollision(this, otherScript, contact);
        OutputDebugStringA("Returned from collisionTypes->ProcessCollision\n");
    }
    else
    {
        OutputDebugStringA("No collision types handler!\n");
    }
    
    // Set cooldown (only if objects are still valid)
    if (!markedForDestroy && !otherScript->IsMarkedForDestroy())
    {
        reactionCooldown = 0.05f;  // 연쇄 반응을 위해 쿨타임 단축
        otherScript->reactionCooldown = 0.05f;
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