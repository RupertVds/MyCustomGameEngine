#include "PlayerComponent.h"
#include "PlayerStates.h"
#include <Timer.h>
#include <Renderer.h>

PlayerComponent::PlayerComponent(GameObject* pOwner)
    :
    Component(pOwner),
    m_StateMachine(this)
{
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

    // Apply acceleration
    m_Velocity += m_Acceleration * Timer::GetInstance().GetFixedTimeStep(); // Assuming m_TimeStep is the fixed time step

    // Apply velocity
    glm::vec2 displacement = m_Velocity * Timer::GetInstance().GetFixedTimeStep(); // Displacement = velocity * time

    // Update position
    // Assuming m_Owner->GetPosition() returns the current position of the GameObject
    glm::vec2 newPosition = glm::vec2(GetOwner()->GetLocalPosition()) + displacement;

    // Set the new position
    // temporary ground collision
    if (newPosition.y >= Renderer::HEIGHT / 1.2)
    {
        GetOwner()->SetLocalPosition({ newPosition.x, Renderer::HEIGHT / 1.2, 0 });
        m_Acceleration = { 0, 0 };
        SetVerticalVelocity(0);
    }
    else
    {
        GetOwner()->SetLocalPosition({ newPosition, 0 });
    }
}

void PlayerComponent::LateUpdate()
{
    m_StateMachine.LateUpdate();
}