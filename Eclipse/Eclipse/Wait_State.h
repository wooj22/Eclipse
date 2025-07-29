#pragma once
#include "ActionStateBase.h"  

class ActionFSM;

class Wait_State : public ActionStateBase
{
public:
    void Enter(ActionFSM* fsm) override;
    void Update(ActionFSM* fsm) override;
    void Exit(ActionFSM* fsm) override;

private:
    float holdTime = 0.0f;
    bool isHolding = false;
    const float bulletTimeThreshold = 0.3f;

    float timer = 0.0f;
    const float ignoreInputDuration = 1.5f; // 입력 무시
};