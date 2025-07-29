#pragma once
#include "FSMBase.h"
#include "MovementStateBase.h"
#include "Idle_State.h"

class PlayerFSM;

class MovementFSM : public FSMBase<MovementFSM, MovementStateBase>
{
private:
    PlayerFSM* playerFSM = nullptr;

public:
    void Init(PlayerFSM* fsm)
    {
        playerFSM = fsm;

        ChangeState(std::make_unique<Idle_State>()); // 기본 상태 Idle
    }
    PlayerFSM* GetPlayerFSM() const { return playerFSM; }
};