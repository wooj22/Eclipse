#pragma once
#include "MovementStateBase.h"

class MovementFSM;

class Jump_State : public MovementStateBase
{
    bool canDoubleJump = false;
    float coyoteTime = 0.15f;   // 점프 직후 잠깐은 착지 판정 무시
    float timer = 0.0f;

public:
    void Enter(MovementFSM* fsm) override;
    void Update(MovementFSM* fsm) override;
    void FixedUpdate(MovementFSM* fsm) override;
    void Exit(MovementFSM* fsm) override;
};