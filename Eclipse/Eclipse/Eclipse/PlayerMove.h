#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/Input.h"

class PlayerMove : public Script
{
private:
	float inputX, inputY, speed = 10;
	Transform* transform;

public: 
	void OnEnable() override;				// 컴포넌트 활성화 시점)
	void Update() override;					// 매 프레임 반복 호출
	void OnCollisionStay(ICollider* other, const ContactInfo& contact) override;
};

