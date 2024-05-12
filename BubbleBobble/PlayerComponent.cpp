#include "PlayerComponent.h"
#include "PlayerStates.h"

PlayerComponent::PlayerComponent(GameObject* pOwner)
    :
    Component(pOwner),
    m_StateMachine(this)
{
    m_pPhysicsComponent = GetOwner()->GetComponent<PhysicsComponent>();
    if (m_pPhysicsComponent == nullptr)
    {
        throw std::runtime_error("PLAYER: MISSING PHYSICS COMPONENT!");
    }

    m_pBody = m_pPhysicsComponent->GetBody();

    // Set the initial state of the player component
    m_StateMachine.SetState(new PlayerEntryState());
}

void PlayerComponent::Update()
{
    m_StateMachine.Update();
}

void PlayerComponent::FixedUpdate()
{
    m_StateMachine.FixedUpdate();
}

void PlayerComponent::LateUpdate()
{
    m_StateMachine.LateUpdate();
}