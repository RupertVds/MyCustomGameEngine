#pragma once
#include "PlayerComponent.h"
#include <iostream>

// Forward declarations
class PlayerComponent;
class PlayerAliveState;
class PlayerDeadState;

// PlayerEntryState
class PlayerEntryState final : public BehaviorState<PlayerComponent> {
public:
    void Entry(BehaviorStateMachine<PlayerComponent>&);
    void Update(BehaviorStateMachine<PlayerComponent>& stateMachine);
    void Exit(BehaviorStateMachine<PlayerComponent>&);
private:
    const float m_MovingDownSpeed = 20.f;
    const float m_MovingDownTime = 5.f;
    float m_MovingDownElapsedTime = 0.f;
};

// PlayerAliveState
class PlayerAliveState final : public BehaviorState<PlayerComponent> {
public:
    void Entry(BehaviorStateMachine<PlayerComponent>&);
    void Update(BehaviorStateMachine<PlayerComponent>& stateMachine);
    //void FixedUpdate(BehaviorStateMachine<PlayerComponent>& stateMachine);
    void LateUpdate(BehaviorStateMachine<PlayerComponent>& stateMachine);
    void Exit(BehaviorStateMachine<PlayerComponent>&);
private:
    float m_MoveSpeed{};
};

// PlayerDeadState
class PlayerDeadState final : public BehaviorState<PlayerComponent> {
public:
    void Entry(BehaviorStateMachine<PlayerComponent>&);
    void Update(BehaviorStateMachine<PlayerComponent>& stateMachine);
    void Exit(BehaviorStateMachine<PlayerComponent>&);
};
