#pragma once
#include "ActionStateBase.h" 

class ActionFSM;

class Attack_State : public ActionStateBase
{
    float timer = 0.0f;

public:
    void Enter(ActionFSM* fsm) override;
    void Update(ActionFSM* fsm) override;
    void Exit(ActionFSM* fsm) override;
};