#include "PlayerComponent.h"
#include "PlayerStates.h"
#include <Timer.h>
#include <Renderer.h>
#include <Utils.h>

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
    
    GetOwner()->SetLocalPosition({ newPosition, 0 });

    // Perform raycast downwards to check if the player is grounded
    glm::vec2 rayOrigin = newPosition;
    glm::vec2 rayDirection(0.0f, 1.0f); // Downwards direction
    float rayDistance = 50.1f;
    RaycastResult result = Raycast(rayOrigin, rayDirection, rayDistance, CollisionComponent::ColliderType::STATIC);
    RenderRaycast(rayOrigin, rayDirection, 60);
    if (result.hit) 
    {
        if (!m_IsJumping)
        {
            m_Acceleration = { 0.f, 9.81f };
            SetVerticalVelocity(0);

        }
        SetIsGrounded(true);
    }
    else 
    {
        // Player is not grounded
        SetIsGrounded(false);
    }

    std::cout << "is player grounded: " << m_IsGrounded << '\n';
}

void PlayerComponent::LateUpdate()
{
    m_StateMachine.LateUpdate();
}