#pragma once
#include "../Direct2D_EngineLib/Script.h"

// 플레이어의 어택 충돌 영역 컨트롤러 

class PlayerAreaController : public Script
{
public:
	// script component cycle
	void Awake() override {}
	void Start() override {}
	void Update() override {}
	void OnDestroy() override{}

	// trigger event
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override
	{
		if (other->gameObject->tag == "Hon")
		{
			OutputDebugStringA("Hon과 충돌 함 \n");
			// this->gameObject->SetActive(false);
		}
	}

	void OnTriggerStay(ICollider* other, const ContactInfo& contact) override
	{
	}
};