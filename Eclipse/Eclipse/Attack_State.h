#pragma once
#include "MovementStateBase.h" 

class MovementFSM;

class Attack_State : public MovementStateBase
{
    float timer = 0.0f;

public:
    void Enter(MovementFSM* fsm) override;
    void Update(MovementFSM* fsm) override;
    void Exit(MovementFSM* fsm) override;

    void FixedUpdate(MovementFSM* fsm) override;
};