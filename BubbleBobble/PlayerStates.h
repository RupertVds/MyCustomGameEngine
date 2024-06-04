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
    void FixedUpdate(BehaviorStateMachine<PlayerComponent>& stateMachine);
    void Exit(BehaviorStateMachine<PlayerComponent>&);
private:
    const float m_MovingDownSpeed = 50.f;
    const float m_MovingDownTime = 1.f;
    float m_MovingDownElapsedTime = 0.f;
};

// PlayerAliveState
class PlayerAliveState final : public BehaviorState<PlayerComponent> {
public:
    void Entry(BehaviorStateMachine<PlayerComponent>&);
    void Update(BehaviorStateMachine<PlayerComponent>& stateMachine);
    void HandleAnimation(PlayerComponent* playerComp);
    void HandleJumping(PlayerComponent* playerComp);
    void HandleHorizontalMovement(PlayerComponent* playerComp);
    void FixedUpdate(BehaviorStateMachine<PlayerComponent>& stateMachine);
    void HandleGround(PlayerComponent* playerComp);
    void LateUpdate(BehaviorStateMachine<PlayerComponent>& stateMachine);
    void Exit(BehaviorStateMachine<PlayerComponent>&);
private:
    float m_MoveSpeed{};
    float m_JumpTimer{};
    float m_JumpTime{};
    float m_JumpSpeed{};
    float m_JumpTimeThreshold{};
    float m_FallingSpeed{};
    // for animation bug..
    float m_FallingTimer{};
    float m_FallingTime{ 0.025f };
};

// PlayerDeadState
class PlayerDeadState final : public BehaviorState<PlayerComponent> {
public:
    void Entry(BehaviorStateMachine<PlayerComponent>&);
    void Update(BehaviorStateMachine<PlayerComponent>& stateMachine);
    void Exit(BehaviorStateMachine<PlayerComponent>&);
};
