#pragma once
#include "HonController.h"

/* [A] Ignis 불의 혼 컨트롤러 Script */
// 연쇄반응
// 1. A : 두 오브젝트가 합쳐짐 (크기 +10%, 하강속도 -20%)
// 2. B : 충돌 반대 방향으로 튕김
// 3. C : 좌우로 튕김
// 4. D : 파괴

class HonAController : public HonController
{
public:
	HonAController()
	{
		hp = 3;
		size = 1;
		descentSpeed = 80;
		collisionSpeed = 150;

		collisionMovingTime = 3.0f;
		pullMovingTime = 0.7f;
	}

	// script component cycle
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	// trigger event
	void OnTriggerEnter(ICollider* other, const ContactInfo& contact) override;
};

