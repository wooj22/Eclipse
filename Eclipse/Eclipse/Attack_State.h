#pragma once
#include "MovementStateBase.h" 
#include "../Direct2D_EngineLib/Vector2.h"

#include "PlayerAttackArea.h"

class MovementFSM;

class Attack_State : public MovementStateBase
{
    float maxDistance = 300.0f;  // �ִ� �̵� �Ÿ�
    float desiredTime = 0.3f;    // ���� �ð�

    float timer = 0.0f;
    float moveSpeed = 0.0f;

    Vector2 startPos;
    Vector2 targetPos;
    Vector2 direction;

    float Lerp(float a, float b, float t) { return a + (b - a) * t; } // �������� 


public:
    void Enter(MovementFSM* fsm) override;
    void Update(MovementFSM* fsm) override;
    void Exit(MovementFSM* fsm) override;

    void FixedUpdate(MovementFSM* fsm) override;
};