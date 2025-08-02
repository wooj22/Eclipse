#include "HonmunKnockbackTest.h"
#include "Honmun.h"
#include "../Direct2D_EngineLib/Vector2.h"
#include "../Direct2D_EngineLib/Transform.h"
#include "../Direct2D_EngineLib/Rigidbody.h"

void HonmunKnockbackTest::Awake()
{
    OutputDebugStringA("HonmunKnockbackTest Awake - Aron Scene Test Script!\n");
}

void HonmunKnockbackTest::OnTriggerEnter(ICollider* other, const ContactInfo& contact)
{
    OutputDebugStringA("HonmunKnockbackTest::OnTriggerEnter called!\n");
    
    // 더 안전한 Honmun 감지 방법 (양우정님 엔진 가이드)
    if (!other || !other->gameObject) {
        OutputDebugStringA("Invalid collision object\n");
        return;
    }
    
    // GameObject 이름으로 Honmun 확인 (2A, b 타입 포함)
    std::string objName = other->gameObject->name;
    if (objName.find("Honmun") == std::string::npos && 
        objName.find("Merged_2A") == std::string::npos &&
        objName.find("2A") == std::string::npos) {
        char debugMsg[100];
        sprintf_s(debugMsg, "Not a Honmun object: %s - collision ignored\n", objName.c_str());
        OutputDebugStringA(debugMsg);
        return;
    }
    
    // 2A 타입 감지 로그 추가
    char detectionMsg[100];
    sprintf_s(detectionMsg, "Player attack detected object: %s\n", objName.c_str());
    OutputDebugStringA(detectionMsg);
    
    // Honmun으로 캐스팅 시도
    auto* otherHonmun = dynamic_cast<Honmun*>(other->gameObject);
    if (!otherHonmun) 
    {
        OutputDebugStringA("Failed to cast to Honmun - trying GameObject-based knockback\n");
        
        // dynamic_cast 실패 시 GameObject로 직접 처리
        auto* gameObj = other->gameObject;
        char debugMsg[100];
        sprintf_s(debugMsg, "Player attack hit GameObject: %s!\n", gameObj->name.c_str());
        OutputDebugStringA(debugMsg);
        
        // 안전한 Transform과 Rigidbody 접근 (양우정님 가이드)
        auto* transform = gameObj->GetComponent<Transform>();
        auto* rigidbody = gameObj->GetComponent<Rigidbody>();
        auto* attackTransform = gameObject->GetComponent<Transform>();
        
        if (transform && rigidbody && attackTransform) {
            Vector2 attackPos = attackTransform->GetPosition();
            Vector2 targetPos = transform->GetPosition();
            Vector2 knockbackDirection = (targetPos - attackPos).Normalized();
            
            rigidbody->isKinematic = false;
            rigidbody->useGravity = false;
            
            // 훨씬 더 강한 넉백 (눈에 확실히 보이도록)
            float knockbackForce = 1500.0f; // 4배 증가!
            Vector2 knockbackImpulse = knockbackDirection * knockbackForce;
            rigidbody->AddImpulse(knockbackImpulse); // AddImpulse로 변경 (더 강함)
            
            OutputDebugStringA("Knockback applied to GameObject by HonmunKnockbackTest!\n");
        }
        return;
    }
    
    char debugMsg[100];
    sprintf_s(debugMsg, "Player attack hit Honmun: %s!\n", otherHonmun->name.c_str());
    OutputDebugStringA(debugMsg);
    
    // 안전한 Transform 접근 (양우정님 가이드)
    auto* attackTransform = gameObject->GetComponent<Transform>();
    auto* honmunTransform = otherHonmun->GetComponent<Transform>();
    
    if (!attackTransform || !honmunTransform) 
    {
        OutputDebugStringA("Missing transform components!\n");
        return;
    }
    
    Vector2 attackPos = attackTransform->GetPosition();
    Vector2 honmunPos = honmunTransform->GetPosition();
    
    // 밀어낼 방향 계산
    Vector2 knockbackDirection = (honmunPos - attackPos).Normalized();
    
    // 안전한 Rigidbody 접근 (양우정님 가이드)
    auto* honmunRb = otherHonmun->GetComponent<Rigidbody>();
    if (honmunRb)
    {
        // 물리 모드로 전환하고 넉백 적용
        honmunRb->isKinematic = false;
        honmunRb->useGravity = false;
        
        // 훨씬 더 강한 넉백 (눈에 확실히 보이도록)
        float knockbackForce = 1500.0f; // 4배 증가!
        Vector2 knockbackImpulse = knockbackDirection * knockbackForce;
        honmunRb->AddImpulse(knockbackImpulse); // AddImpulse로 변경 (더 강함)
        
        OutputDebugStringA("Knockback applied to Honmun by HonmunKnockbackTest!\n");
    }
    else
    {
        OutputDebugStringA("Honmun has no Rigidbody component!\n");
    }
}