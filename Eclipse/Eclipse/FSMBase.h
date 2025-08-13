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
        {
            currentState->Exit(static_cast<OwnerType*>(this));
            previousState = currentState.get(); // 이전 상태 저장
        }
        else
        {
            previousState = nullptr;
        }

        currentState = std::move(newState);

        if (currentState)
            currentState->Enter(static_cast<OwnerType*>(this));
    }

    StateType* GetCurrentState() const
    {
        return currentState.get();
    }

    StateType* GetPreviousState() const
    {
        return previousState;
    }
};