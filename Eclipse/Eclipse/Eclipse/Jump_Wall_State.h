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
    float wallJumpForce = 350.0f;
    float jumpXPower = 300.0f;
};