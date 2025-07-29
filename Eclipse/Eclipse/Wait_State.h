#pragma once
#include "ActionStateBase.h"  

class ActionFSM;

class Wait_State : public ActionStateBase
{
public:
    void Enter(ActionFSM* fsm) override;
    void Update(ActionFSM* fsm) override;
    void Exit(ActionFSM* fsm) override;
};