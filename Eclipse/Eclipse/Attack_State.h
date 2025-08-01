#pragma once
#include "MovementStateBase.h" 
#include "../Direct2D_EngineLib/Vector2.h"

#include "PlayerAttackArea.h"

class MovementFSM;

class Attack_State : public MovementStateBase
{
    float maxDistance = 300.0f;  // 최대 이동 거리
    float desiredTime = 0.3f;    // 도달 시간

    float timer = 0.0f;
    float moveSpeed = 0.0f;

    Vector2 startPos;
    Vector2 targetPos;
    Vector2 direction;

    float Lerp(float a, float b, float t) { return a + (b - a) * t; } // 선형보간 


public:
    void Enter(MovementFSM* fsm) override;
    void Update(MovementFSM* fsm) override;
    void Exit(MovementFSM* fsm) override;

    void FixedUpdate(MovementFSM* fsm) override;
};