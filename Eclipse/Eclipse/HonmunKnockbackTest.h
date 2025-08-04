#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/ICollider.h"

// 아론 씬 전용 혼문 넉백 테스트 스크립트
class HonmunKnockbackTest : public Script
{
public:
    void Awake() override;
    void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;
};