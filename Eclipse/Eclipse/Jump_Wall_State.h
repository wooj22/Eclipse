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
    float jumpXPower = 500.0f;
    float doubleJumpXPower = 200.0f;

    float elapsedTime = 0.0f;       // 경과 시간
    float inputBlockTime = 0.05f;   // 입력 무시 시간
    // float hangingBlockTime = 0.3f;  // 매달리기 제한 시간

    bool canDoubleJump = false;

    float airAcceleration = 20.0f;  // 공중 가속도
    float airFriction = 3.0f;       // 공중 감속도

    float inputX;
    float curVelX;

    int lastWallDir = 0; // -1: 왼쪽 벽, 1: 오른쪽 벽

    float Lerp(float a, float b, float t) { return a + (b - a) * t; } // 선형보간 
};