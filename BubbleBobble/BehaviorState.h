#pragma once

template<typename T>
class BehaviorStateMachine;

// Base BehaviorState class
template<typename T>
class BehaviorState
{
public:
    virtual ~BehaviorState() = default;
    virtual void Entry(BehaviorStateMachine<T>&) {};
    virtual void Render(BehaviorStateMachine<T>&) {}
    virtual void Update(BehaviorStateMachine<T>&) {};
    virtual void FixedUpdate(BehaviorStateMachine<T>&) {};
    virtual void LateUpdate(BehaviorStateMachine<T>&) {};
    virtual void Exit(BehaviorStateMachine<T>&) {};
};