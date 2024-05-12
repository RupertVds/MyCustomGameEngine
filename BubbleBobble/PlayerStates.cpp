#include "PlayerStates.h"

// Definition of methods for PlayerEntryState
void PlayerEntryState::Entry(BehaviorStateMachine<PlayerComponent>& stateMachine) 
{
    std::cout << "PlayerEntryState: Entered" << '\n';
    stateMachine.GetComponent()->GetBody()->SetType(b2_kinematicBody);
    std::cout << "PlayerEntryState: Set to kinematic body" << '\n';
    stateMachine.GetComponent()->GetBody()->SetLinearVelocity({ 0, m_MovingDownSpeed });
    std::cout << "PlayerEntryState: Set moving down speed" << '\n';
}

void PlayerEntryState::Update(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    std::cout << "PlayerEntryState: Updated" << std::endl;
    m_MovingDownElapsedTime += Timer::GetInstance().GetDeltaTime();
    if (m_MovingDownElapsedTime >= m_MovingDownTime)
    {
        stateMachine.SetState(new PlayerAliveState());
    }
}

void PlayerEntryState::Exit(BehaviorStateMachine<PlayerComponent>&) 
{
    std::cout << "PlayerEntryState: Exited" << std::endl;
}

// Definition of methods for PlayerAliveState
void PlayerAliveState::Entry(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    std::cout << "PlayerAliveState: Entered" << std::endl;
    stateMachine.GetComponent()->GetBody()->SetType(b2_dynamicBody);
    std::cout << "PlayerEntryState: Set to dynamic body" << '\n';
    m_MovementForce = 50000000;
    m_CounterMovementForce = 300000;
}

void PlayerAliveState::Update(BehaviorStateMachine<PlayerComponent>&)
{
}

void PlayerAliveState::FixedUpdate(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    b2Body* body = stateMachine.GetComponent()->GetBody();
    b2Vec2 counterMovement = { -body->GetLinearVelocity().x * m_CounterMovementForce, 0 };
    b2Vec2 movement = stateMachine.GetComponent()->GetMovingDirection();
    movement *= m_MovementForce;
    body->ApplyForceToCenter({ movement.x + counterMovement.x, 0.f}, false);
}

void PlayerAliveState::LateUpdate(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    stateMachine.GetComponent()->SetMovingDirection({ 0, 0 });
}

void PlayerAliveState::Exit(BehaviorStateMachine<PlayerComponent>&)
{
    std::cout << "PlayerAliveState: Exited" << std::endl;
}

// Definition of methods for PlayerDeadState
void PlayerDeadState::Entry(BehaviorStateMachine<PlayerComponent>&)
{
    std::cout << "PlayerDeadState: Entered" << std::endl;
}

void PlayerDeadState::Update(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    std::cout << "PlayerDeadState: Updated" << std::endl;
    stateMachine.SetState(new PlayerEntryState());
}

void PlayerDeadState::Exit(BehaviorStateMachine<PlayerComponent>&) 
{
    std::cout << "PlayerDeadState: Exited" << std::endl;
}