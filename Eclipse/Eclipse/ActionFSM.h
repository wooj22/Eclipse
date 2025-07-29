#pragma once
#include "FSMBase.h"
#include "ActionStateBase.h"

class PlayerFSM;

class ActionFSM : public FSMBase<ActionFSM, ActionStateBase>
{
private:
    PlayerFSM* playerFSM = nullptr;

public:
    void Init(PlayerFSM* fsm)
    {
        playerFSM = fsm;
    }
    PlayerFSM* GetPlayerFSM() const { return playerFSM; }
};