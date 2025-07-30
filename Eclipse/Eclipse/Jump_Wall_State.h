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
    float jumpXPower = 400.0f;

    float elapsedTime = 0.0f;       // ��� �ð�
    float inputBlockTime = 0.05f;   // �Է� ���� �ð�
    // float hangingBlockTime = 0.3f;  // �Ŵ޸��� ���� �ð�

    float airAcceleration = 20.0f;  // ���� ���ӵ�
    float airFriction = 3.0f;       // ���� ���ӵ�

    float inputX;
    float curVelX;

    int lastWallDir = 0; // -1: ���� ��, 1: ������ ��

    float Lerp(float a, float b, float t) { return a + (b - a) * t; } // �������� 
};