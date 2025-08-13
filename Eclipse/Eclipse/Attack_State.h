#pragma once
#include "MovementStateBase.h" 
#include "../Direct2D_EngineLib/Vector2.h"

#include "PlayerAttackArea.h"

class MovementFSM;

class Attack_State : public MovementStateBase
{
    float baseMaxDistance = 0.0f;    // 최대 이동 거리 (playerFSM에서 가져옴)
    float desiredTime = 0.0f;        // 도달 시간

    float timer = 0.0f;
    float moveSpeed = 0.0f;

    bool airAttack = false;

    Vector2 startPos;
    Vector2 targetPos;
    Vector2 direction;

	bool isAttackStart = false; // 공격 시작 여부

    float Lerp(float a, float b, float t) { return a + (b - a) * t; } // 선형보간 

    // 잔상
    float afterimageTimer = 0.0f;
    float afterimageInterval = 0.015f;
    void CreateAfterImage(MovementFSM* fsm);

public:
    void Enter(MovementFSM* fsm) override;
    void Update(MovementFSM* fsm) override;
    void Exit(MovementFSM* fsm) override;

    void FixedUpdate(MovementFSM* fsm) override;
};