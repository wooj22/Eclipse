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
            
            // 실제 플레이어 위치 찾기 (GameObject 버전)
            Vector2 playerPos = attackPos; // 기본값은 attackPos
            
            // PlayerAttackArea는 플레이어보다 Y축으로 약 50픽셀 위에 있음
            if (gameObject->name == "PlayerAttackArea") {
                playerPos.y = attackPos.y - 50.0f; // 플레이어 실제 위치 추정
                
                char posMsg[100];
                sprintf_s(posMsg, "GameObject - Player estimated pos: (%.1f, %.1f), Attack pos: (%.1f, %.1f)\n", 
                         playerPos.x, playerPos.y, attackPos.x, attackPos.y);
                OutputDebugStringA(posMsg);
            }
            
            // 플레이어의 실제 이동 방향을 기준으로 혼문 넉백 방향 계산
            Vector2 basicDirection;
            
            // GameObject::Find로 Player 객체를 찾아서 velocity 확인
            Rigidbody* playerRigidbody = nullptr;
            auto* playerObject = GameObject::Find("Player");
            if (playerObject) {
                playerRigidbody = playerObject->GetComponent<Rigidbody>();
            }
            
            if (playerRigidbody && playerRigidbody->velocity.Magnitude() > 0.1f) {
                // 플레이어가 이동 중이면 플레이어의 이동 방향 사용
                basicDirection = playerRigidbody->velocity.Normalized();
                OutputDebugStringA("GameObject - Using player velocity direction for knockback\n");
            } else {
                // 플레이어가 정지 상태면 위치 기반 방향 사용
                basicDirection = (targetPos - playerPos).Normalized();
                OutputDebugStringA("GameObject - Using position-based direction for knockback\n");
            }
            
            // 약간의 변화를 주어 연쇄반응 유도
            static int gameObjKnockbackCounter = 0;
            gameObjKnockbackCounter++;
            
            float angleVariation = (gameObjKnockbackCounter % 3 - 1) * 0.2f; // -0.2, 0, 0.2 라디안씩 변화
            float cosVar = cos(angleVariation);
            float sinVar = sin(angleVariation);
            
            // 회전 매트릭스 적용하여 약간의 방향 변화
            Vector2 knockbackDirection;
            knockbackDirection.x = basicDirection.x * cosVar - basicDirection.y * sinVar;
            knockbackDirection.y = basicDirection.x * sinVar + basicDirection.y * cosVar;
            knockbackDirection = knockbackDirection.Normalized();
            
            char dirMsg[100];
            sprintf_s(dirMsg, "GameObject forward knockback (variation: %.2f)\n", angleVariation);
            OutputDebugStringA(dirMsg);
            
            rigidbody->isKinematic = false;
            rigidbody->useGravity = false;
            
            // 연쇄반응을 위한 넉백 강도 조절 (GameObject 버전)
            float baseKnockbackForce = 1500.0f;
            float speedMultiplier = 1.0f;
            
            // 약간의 강도 변화로 연쇄반응 유도
            float forceVariation = 0.8f + (gameObjKnockbackCounter % 5) * 0.1f; // 0.8~1.2 범위
            speedMultiplier = forceVariation;
            
            float knockbackForce = baseKnockbackForce * speedMultiplier;
            Vector2 knockbackImpulse = knockbackDirection * knockbackForce;
            rigidbody->AddImpulse(knockbackImpulse);
            
            char forceMsg[100];
            sprintf_s(forceMsg, "GameObject knockback force: %.1f (multiplier: %.2f)\n", knockbackForce, speedMultiplier);
            OutputDebugStringA(forceMsg);
            
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
    
    // 실제 플레이어 위치 찾기 (PlayerAttackArea의 부모의 부모)
    Vector2 playerPos = attackPos; // 기본값은 attackPos
    
    // 이름으로 실제 플레이어 위치 추정 (PlayerAttackArea는 플레이어 위에 있음)
    if (gameObject->name == "PlayerAttackArea") {
        // PlayerAttackArea는 플레이어보다 Y축으로 약 50픽셀 위에 있음 (30 + 20 오프셋)
        playerPos.y = attackPos.y - 50.0f; // 플레이어 실제 위치 추정
        
        char posMsg[100];
        sprintf_s(posMsg, "Player estimated pos: (%.1f, %.1f), Attack pos: (%.1f, %.1f)\n", 
                 playerPos.x, playerPos.y, attackPos.x, attackPos.y);
        OutputDebugStringA(posMsg);
    }
    
    // 플레이어의 실제 이동 방향을 기준으로 혼문 넉백 방향 계산
    Vector2 basicDirection;
    
    // GameObject::Find로 Player 객체를 찾아서 velocity 확인
    Rigidbody* playerRigidbody = nullptr;
    auto* playerObject = GameObject::Find("Player");
    if (playerObject) {
        playerRigidbody = playerObject->GetComponent<Rigidbody>();
    }
    
    if (playerRigidbody && playerRigidbody->velocity.Magnitude() > 0.1f) {
        // 플레이어가 이동 중이면 플레이어의 이동 방향 사용
        basicDirection = playerRigidbody->velocity.Normalized();
        OutputDebugStringA("Honmun - Using player velocity direction for knockback\n");
    } else {
        // 플레이어가 정지 상태면 위치 기반 방향 사용
        basicDirection = (honmunPos - playerPos).Normalized();
        OutputDebugStringA("Honmun - Using position-based direction for knockback\n");
    }
    
    // 약간의 변화를 주어 연쇄반응 유도 (너무 평행하게 날아가지 않도록)
    static int knockbackCounter = 0;
    knockbackCounter++;
    
    float angleVariation = (knockbackCounter % 3 - 1) * 0.2f; // -0.2, 0, 0.2 라디안씩 변화
    float cosVar = cos(angleVariation);
    float sinVar = sin(angleVariation);
    
    // 회전 매트릭스 적용하여 약간의 방향 변화
    Vector2 knockbackDirection;
    knockbackDirection.x = basicDirection.x * cosVar - basicDirection.y * sinVar;
    knockbackDirection.y = basicDirection.x * sinVar + basicDirection.y * cosVar;
    knockbackDirection = knockbackDirection.Normalized();
    
    char dirMsg[100];
    sprintf_s(dirMsg, "Forward knockback applied (variation: %.2f)\n", angleVariation);
    OutputDebugStringA(dirMsg);
    
    // 안전한 Rigidbody 접근 (양우정님 가이드)
    auto* honmunRb = otherHonmun->GetComponent<Rigidbody>();
    if (honmunRb)
    {
        // 물리 모드로 전환하고 넉백 적용
        honmunRb->isKinematic = false;
        honmunRb->useGravity = false;
        // 연쇄반응을 위한 넉백 강도 조절
        float baseKnockbackForce = 1500.0f;
        float speedMultiplier = 1.0f;
        
        // 약간의 강도 변화로 연쇄반응 유도 (다른 속도로 날아가게)
        float forceVariation = 0.8f + (knockbackCounter % 5) * 0.1f; // 0.8~1.2 범위
        speedMultiplier = forceVariation;
        
        float knockbackForce = baseKnockbackForce * speedMultiplier;
        Vector2 knockbackImpulse = knockbackDirection * knockbackForce;
        honmunRb->AddImpulse(knockbackImpulse);
        
        char forceMsg[100];
        sprintf_s(forceMsg, "Honmun knockback force: %.1f (multiplier: %.2f)\n", knockbackForce, speedMultiplier);
        OutputDebugStringA(forceMsg);
        
        OutputDebugStringA("Forward knockback applied to Honmun by HonmunKnockbackTest!\n");
    }
    else
    {
        OutputDebugStringA("Honmun has no Rigidbody component!\n");
    }
}