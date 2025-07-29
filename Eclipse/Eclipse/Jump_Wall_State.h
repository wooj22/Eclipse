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

    // 점프 중 공중 이동: 부드럽게 가속/감속
    float inputX;
    float airAcceleration = 20.0f;     // 공중 가속도
    float airFriction = 3.0f;       // 공중 감속도

    float curVelX;

    float Lerp(float a, float b, float t) { return a + (b - a) * t; } // 선형보간 
};