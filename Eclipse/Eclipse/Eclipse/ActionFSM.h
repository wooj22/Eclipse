#pragma once
#include "FSMBase.h"
#include "ActionStateBase.h"
#include "Wait_State.h"

class PlayerFSM;

class ActionFSM : public FSMBase<ActionFSM, ActionStateBase>
{
private:
    PlayerFSM* playerFSM = nullptr;

public:
    void Init(PlayerFSM* fsm)
    {
        playerFSM = fsm;

        ChangeState(std::make_unique<Wait_State>()); // 기본 상태 Wait
    }
    PlayerFSM* GetPlayerFSM() const { return playerFSM; }
};