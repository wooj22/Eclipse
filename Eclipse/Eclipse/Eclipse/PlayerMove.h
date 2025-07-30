#pragma once
#include "../Direct2D_EngineLib/Script.h"
#include "../Direct2D_EngineLib/Input.h"

class PlayerMove : public Script
{
private:
	float inputX, inputY, speed = 10;
	Transform* transform;

public: 
	void OnEnable() override;				// ������Ʈ Ȱ��ȭ ����)
	void Update() override;					// �� ������ �ݺ� ȣ��
	void OnCollisionStay(ICollider* other, const ContactInfo& contact) override;
};

