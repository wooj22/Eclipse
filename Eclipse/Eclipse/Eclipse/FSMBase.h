#pragma once
#include <memory>
#include "StateBase.h"

template<typename OwnerType, typename StateType>
class FSMBase
{
protected:
    std::unique_ptr<StateType> currentState;

public:
    virtual ~FSMBase() = default;

    void Update()
    {
        if (currentState)
            currentState->Update(static_cast<OwnerType*>(this));
    }

    void FixedUpdate()
    {
        if (currentState)
            currentState->FixedUpdate(static_cast<OwnerType*>(this));
    }

    void ChangeState(std::unique_ptr<StateType> newState)
    {
        if (currentState)
            currentState->Exit(static_cast<OwnerType*>(this));

        currentState = std::move(newState);

        if (currentState)
            currentState->Enter(static_cast<OwnerType*>(this));
    }
};