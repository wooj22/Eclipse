#pragma once
#include "../Direct2D_EngineLib/Script.h"

// �÷��̾��� ���� �浹 ���� ��Ʈ�ѷ� 

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
			OutputDebugStringA("Hon�� �浹 �� \n");
			// this->gameObject->SetActive(false);
		}
	}

	void OnTriggerStay(ICollider* other, const ContactInfo& contact) override
	{
	}
};