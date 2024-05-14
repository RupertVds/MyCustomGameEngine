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
    const float m_MovingDownSpeed = 50.f;
    const float m_MovingDownTime = 3.f;
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
    float m_JumpTimer{};
    float m_JumpTime{};
    float m_JumpSpeed{};
    float m_JumpTimeThreshold{};
    bool m_ReachedTopOfJump{};
    float m_FallingSpeed{};
};

// PlayerDeadState
class PlayerDeadState final : public BehaviorState<PlayerComponent> {
public:
    void Entry(BehaviorStateMachine<PlayerComponent>&);
    void Update(BehaviorStateMachine<PlayerComponent>& stateMachine);
    void Exit(BehaviorStateMachine<PlayerComponent>&);
};
