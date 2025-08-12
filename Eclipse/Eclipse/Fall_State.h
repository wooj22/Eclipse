#pragma once
#include "MovementStateBase.h"

class MovementFSM;

class Fall_State : public MovementStateBase
{
    // 공중 이동: 부드럽게 가속/감속
    float inputX;
    float airAcceleration = 30.0f;     // 공중 가속도
    float airFriction = 3.0f;          // 공중 감속도

    float curVelX;

    float Lerp(float a, float b, float t) { return a + (b - a) * t; } // 선형보간 

    // 잔상
    float afterimageTimer = 0.0f;
    float afterimageInterval = 0.01f;
    void CreateAfterImage(MovementFSM* fsm);

public:
    void Enter(MovementFSM* fsm) override;
    void Update(MovementFSM* fsm) override;
    void Exit(MovementFSM* fsm) override;

    void FixedUpdate(MovementFSM* fsm) override;
};