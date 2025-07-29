#pragma once
#include "MovementStateBase.h"

class Jump_Wall_State : public MovementStateBase
{
public:
    void Enter(MovementFSM* fsm) override;
    void Update(MovementFSM* fsm) override;
    void FixedUpdate(MovementFSM* fsm) override;
    void Exit(MovementFSM* fsm) override;

private:
    float wallJumpForce;
    float jumpXPower = 450.0f;

    // ���� �� ���� �̵�: �ε巴�� ����/����
    float inputX;
    float airAcceleration = 20.0f;     // ���� ���ӵ�
    float airFriction = 3.0f;       // ���� ���ӵ�

    float curVelX;

    float Lerp(float a, float b, float t) { return a + (b - a) * t; } // �������� 
};