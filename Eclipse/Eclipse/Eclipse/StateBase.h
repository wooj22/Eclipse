#pragma once

template<typename OwnerType>
class StateBase
{
public:
    virtual ~StateBase() = default;
    virtual void Enter(OwnerType* fsm) = 0;
    virtual void Update(OwnerType* fsm) = 0;
    virtual void Exit(OwnerType* fsm) = 0;
};