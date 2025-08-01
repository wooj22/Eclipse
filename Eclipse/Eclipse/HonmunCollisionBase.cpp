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
    }
}

void HonmunCollisionBase::Start()
{
    // Additional initialization after all components are set up
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
    
    // Handle physics transition for split fragments
    if (needsPhysicsTransition)
    {
        splitPhysicsTimer += Time::GetDeltaTime();
        if (splitPhysicsTimer >= 0.1f) // 0.1초 후 물리 전환
        {
            if (rigidbody)
            {
                rigidbody->useGravity = true;
                rigidbody->isKinematic = false;
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
    if (!collisionManager) return;
    
    // Get other collision script
    HonmunCollisionBase* otherScript = collisionManager->GetHonmunScript(other);
    if (!otherScript) return;
    
    // Prevent processing if either is already processing
    if (isProcessingReaction || otherScript->IsProcessingReaction()) return;
    
    // Set processing flag
    isProcessingReaction = true;
    otherScript->SetProcessingReaction(true);
    
    // Delegate to collision types handler
    if (collisionTypes)
    {
        collisionTypes->ProcessCollision(this, otherScript, contact);
    }
    
    // Set cooldown
    reactionCooldown = 0.1f;
    otherScript->reactionCooldown = 0.1f;
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
    if (collisionManager)
    {
        collisionManager->DestroyHonmun(this);
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