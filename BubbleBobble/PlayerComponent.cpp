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
    m_pCollider = GetOwner()->GetComponent<BoxColliderComponent>();
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

    HandleGroundCheck(newPosition);
}

void PlayerComponent::HandleGroundCheck(const glm::vec2& newPosition)
{
    // Perform raycast downwards to check if the player is grounded
    glm::vec2 rayOrigin = newPosition;
    rayOrigin.x += m_pCollider->GetWidth() * 0.5f;
    glm::vec2 rayDirection(0.0f, 1.0f); // Downwards direction
    float rayDistance = m_pCollider->GetHeight() + 0.01f;
    RaycastResult result = Raycast(rayOrigin, rayDirection, rayDistance, CollisionComponent::ColliderType::STATIC);
    if (result.hit)
    {
        std::cout << "raycast hit x: " << result.point.x << '\n';
        if (!m_IsJumping)
        {
            m_Acceleration = { 0.f, 9.81f };
            SetVerticalVelocity(0);

        }
        SetIsGrounded(true);
        std::cout << "is player grounded: " << m_IsGrounded << '\n';
    }
    else
    {
        // Player is not grounded
        SetIsGrounded(false);
        std::cout << "is player grounded: " << m_IsGrounded << '\n';
    }
}

void PlayerComponent::LateUpdate()
{
    m_StateMachine.LateUpdate();
}