#pragma once
#include "MovementStateBase.h"

class MovementFSM;

class Dash_State : public MovementStateBase
{
private:
    float dashTimer = 0.0f;
    float dashDuration = 0.2f;

    float moveBonus = 0.0f;
    float dashSpeed = 0.0;
    int inputX = 0.0f;

    float targetYPosition = 0.0f;
    float riseAmount = -15.0f; // 위로 얼마나 뜰지 (픽셀 단위)

public:
    void Enter(MovementFSM* fsm) override;
    void Update(MovementFSM* fsm) override;
    void FixedUpdate(MovementFSM* fsm) override;
    void Exit(MovementFSM* fsm) override;
};