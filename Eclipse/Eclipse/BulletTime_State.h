#pragma once
#include "MovementStateBase.h"
#include "../Direct2D_EngineLib/SpriteRenderer.h"

class MovementFSM;

class BulletTime_State : public MovementStateBase
{
    // ¿‹ªÛ
    float afterimageTimer = 0.0f;
    float afterimageInterval = 0.04f;
    void CreateAfterImage(MovementFSM* fsm);

    SpriteRenderer* blackOut_renderer = nullptr;

public:
    void Enter(MovementFSM* fsm) override;
    void Update(MovementFSM* fsm) override;
    void Exit(MovementFSM* fsm) override;

    void FixedUpdate(MovementFSM* fsm) override;
};