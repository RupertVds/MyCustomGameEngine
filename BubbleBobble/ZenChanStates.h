#pragma once
#include "ZenChanComponent.h"

// Forward declarations
class ZenChanEntryState;
class ZenChanWanderState;

class ZenChanEntryState final : public BehaviorState<ZenChanComponent> {
public:
    void Entry(BehaviorStateMachine<ZenChanComponent>&);
    void Update(BehaviorStateMachine<ZenChanComponent>& stateMachine);
    void FixedUpdate(BehaviorStateMachine<ZenChanComponent>& stateMachine);
    void Exit(BehaviorStateMachine<ZenChanComponent>&);
private:
    void HandleGround(ZenChanComponent* zenChanComp);
    const float m_MovingDownSpeed = 50.f;
    const float m_MovingDownTime = 1.f;
    float m_MovingDownElapsedTime = 0.f;
};

class ZenChanWanderState final : public BehaviorState<ZenChanComponent> {
public:
    void Entry(BehaviorStateMachine<ZenChanComponent>&);
    void Update(BehaviorStateMachine<ZenChanComponent>& stateMachine);
    void FixedUpdate(BehaviorStateMachine<ZenChanComponent>& stateMachine);
    void LateUpdate(BehaviorStateMachine<ZenChanComponent>& stateMachine);
    void Exit(BehaviorStateMachine<ZenChanComponent>&);
private:
    void HandleGround(ZenChanComponent* zenChanComp);
    void HandleJumping(ZenChanComponent* zenChanComp);
    void HandleHorizontalMovement(ZenChanComponent* zenChanComp);
    float m_MoveSpeed = 100.0f;
    float m_JumpDelayTime = 2.0f;
    float m_JumpDelayElapsedTime = 0.0f;
    float m_JumpTime{ 0.35f };
    float m_JumpElapsedTime = 0.0f;
    float m_JumpSpeed{ 240.f };
    float m_FallingSpeed{ 140.f };
    bool m_IsJumping{};
    bool m_IsUnderCeiling{};

    float m_MinJumpInterval = 0.5f; // Minimum interval between jumps
    float m_MaxJumpInterval = 4.0f; // Maximum interval between jumps
    float m_CurrentJumpInterval = 0.0f;
};