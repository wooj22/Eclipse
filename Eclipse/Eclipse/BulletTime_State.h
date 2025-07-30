#pragma once
#include "MovementStateBase.h"

class MovementFSM;

class BulletTime_State : public MovementStateBase
{
public:
    void Enter(MovementFSM* fsm) override;
    void Update(MovementFSM* fsm) override;
    void Exit(MovementFSM* fsm) override;

    void FixedUpdate(MovementFSM* fsm) override;

private:
    float timer = 0.0f;
    const float bulletTimeDuration = 1.0f;
};