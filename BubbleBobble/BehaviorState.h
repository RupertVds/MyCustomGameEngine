#pragma once

template<typename T>
class BehaviorStateMachine;

// Base BehaviorState class
template<typename T>
class BehaviorState {
public:
    virtual ~BehaviorState() = default;
    virtual void Entry(BehaviorStateMachine<T>& stateMachine) = 0;
    virtual void Render(BehaviorStateMachine<T>&) {}
    virtual void Update(BehaviorStateMachine<T>& stateMachine) = 0;
    virtual void Exit(BehaviorStateMachine<T>& stateMachine) = 0;
};