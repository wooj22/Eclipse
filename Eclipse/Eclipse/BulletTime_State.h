#pragma once
#include "StateBase.h"
class ActionFSM;

class BulletTime_State : public StateBase<ActionFSM>
{
public:
    void Enter(ActionFSM* fsm) override;
    void Update(ActionFSM* fsm) override;
    void Exit(ActionFSM* fsm) override;

private:
    float timer = 0.0f;
    const float bulletTimeDuration = 1.0f;
};