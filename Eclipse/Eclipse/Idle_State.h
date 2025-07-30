#pragma once
#include "MovementStateBase.h"

class MovementFSM;

class Idle_State : public MovementStateBase
{
private:
    float holdTime = 0.0f;
    bool isHolding = false;
    const float bulletTimeThreshold = 0.3f;

    float timer = 0.0f;
    const float ignoreInputDuration = 1.5f; // 입력 무시

public:
    void Enter(MovementFSM* fsm) override;
    void Update(MovementFSM* fsm) override;
    void Exit(MovementFSM* fsm) override;

    void FixedUpdate(MovementFSM* fsm) override;
};