#pragma once
#include "MovementStateBase.h"

class MovementFSM;

class Dash_State : public MovementStateBase
{
private:
    float dashTimer = 0.0f;
    float dashDuration = 0.3f;

    float moveBonus = 0.0f;
    float dashSpeed = 0.0;
    int inputX = 0.0f;

    float targetYPosition = 0.0f;
    float riseAmount = -15.0f; // À§·Î ¾ó¸¶³ª ¶ãÁö (ÇÈ¼¿ ´ÜÀ§)

    // ÀÜ»ó
    float afterimageTimer = 0.0f;
    float afterimageInterval = 0.02f;

public:
    void Enter(MovementFSM* fsm) override;
    void Update(MovementFSM* fsm) override;
    void FixedUpdate(MovementFSM* fsm) override;
    void Exit(MovementFSM* fsm) override;

    void CreateAfterImage(MovementFSM* fsm);
};