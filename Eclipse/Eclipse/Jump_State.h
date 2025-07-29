#pragma once
#include "MovementStateBase.h"


namespace Math
{
    float Lerp(float a, float b, float t) { return a + (b - a) * t; } // �������� 
}

class MovementFSM;

class Jump_State : public MovementStateBase
{
    bool canDoubleJump = false;
    float coyoteTime = 0.15f;   // ���� ���� ����� ���� ���� ����
    float timer = 0.0f;

    // ���� �� ���� �̵�: �ε巴�� ����/����
    float inputX;
    float airAcceleration = 30.0f;     // ���� ���ӵ�
    float airFriction = 5.0f;       // ���� ���ӵ�

    float curVelX;

public:
    void Enter(MovementFSM* fsm) override;
    void Update(MovementFSM* fsm) override;
    void FixedUpdate(MovementFSM* fsm) override;
    void Exit(MovementFSM* fsm) override;
};