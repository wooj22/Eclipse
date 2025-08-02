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
    // Get component references
    honmun = dynamic_cast<Honmun*>(gameObject);
    transform = gameObject->GetComponent<Transform>();
    rigidbody = gameObject->GetComponent<Rigidbody>();
    spriteRenderer = gameObject->GetComponent<SpriteRenderer>();
    
    // Initialize properties
    if (honmun)
    {
        honmunType = honmun->GetHonmunType();
        health = honmun->GetHP();
        currentSize = honmun->GetSize();
        
        // 디버그: 혼문 충돌 스크립트 초기화 확인
        char debugMsg[100];
        sprintf_s(debugMsg, "HonmunCollisionBase Awake: Type %d, Name: %s\n", 
                 static_cast<int>(honmunType), gameObject->name.c_str());
        OutputDebugStringA(debugMsg);
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
    
    // Handle physics transition for split fragments - 충돌 테스트용으로 비활성화
    if (needsPhysicsTransition)
    {
        splitPhysicsTimer += Time::GetDeltaTime();
        if (splitPhysicsTimer >= 0.1f) // 0.1초 후 물리 전환
        {
            if (rigidbody)
            {
                rigidbody->useGravity = false; // 충돌 테스트용으로 중력 비활성화
                rigidbody->isKinematic = true; // 키네마틱 모드 유지 (충돌 테스트용)
            }
            needsPhysicsTransition = false;
            splitPhysicsTimer = 0.0f;
        }
    }
    
    // Handle velocity and friction
    if (currentVelocity.Magnitude() > minVelocity)
    {
        currentVelocity *= friction;
    }
    else
    {
        currentVelocity = Vector2(0, 0);
    }
}

void HonmunCollisionBase::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
{
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
    
    // Get other collision script
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
    
    // Prevent processing if either is already processing
    if (isProcessingReaction || otherScript->IsProcessingReaction()) 
    {
        OutputDebugStringA("Collision already processing, skipping\n");
        return;
    }
    
    // 키네마틱 모드에서 충돌 처리 디버그 메시지
    char debugMsg[100];
    sprintf_s(debugMsg, "SUCCESS! Honmun collision: Type %d vs Type %d\n", 
              static_cast<int>(honmunType), static_cast<int>(otherScript->GetHonmunType()));
    OutputDebugStringA(debugMsg);
    
    // Set processing flag BEFORE calling ProcessCollision (but after the check)
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
    
    // Set cooldown
    reactionCooldown = 0.1f;
    otherScript->reactionCooldown = 0.1f;
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
    markedForDestroy = true;
    if (honmun)
    {
        honmun->Destroy();
    }
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