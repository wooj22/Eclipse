#pragma once
#include "MovementStateBase.h"

class MovementFSM;

class Dash_State : public MovementStateBase
{
private:
    float dashTimer = 0.0f;
    float dashDuration = 0.2f;

public:
    void Enter(MovementFSM* fsm) override;
    void Update(MovementFSM* fsm) override;
    void FixedUpdate(MovementFSM* fsm) override;
    void Exit(MovementFSM* fsm) override;
};