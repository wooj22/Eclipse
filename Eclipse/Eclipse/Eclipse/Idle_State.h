#pragma once
#include "MovementStateBase.h"

class MovementFSM;

class Idle_State : public MovementStateBase
{
public:
    void Enter(MovementFSM* fsm) override;
    void Update(MovementFSM* fsm) override;
    void Exit(MovementFSM* fsm) override;

    void FixedUpdate(MovementFSM* fsm) override;
};