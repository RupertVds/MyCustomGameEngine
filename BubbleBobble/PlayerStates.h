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
};

// PlayerAliveState
class PlayerAliveState final : public BehaviorState<PlayerComponent> {
public:
    void Entry(BehaviorStateMachine<PlayerComponent>&);
    void Update(BehaviorStateMachine<PlayerComponent>& stateMachine);
    void Exit(BehaviorStateMachine<PlayerComponent>&);
};

// PlayerDeadState
class PlayerDeadState final : public BehaviorState<PlayerComponent> {
public:
    void Entry(BehaviorStateMachine<PlayerComponent>&);
    void Update(BehaviorStateMachine<PlayerComponent>& stateMachine);
    void Exit(BehaviorStateMachine<PlayerComponent>&);
};
