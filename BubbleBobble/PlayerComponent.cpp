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
    m_pMainCollider = GetOwner()->GetComponent<CircleColliderComponent>();

    auto ceilingTriggerObject = std::make_unique<GameObject>();
    ceilingTriggerObject->AddComponent<BoxColliderComponent>(24.f, 12.f, CollisionComponent::ColliderType::STATIC, true);
    ceilingTriggerObject->SetLocalPosition({ 16.f - 12.f, 0.f});
    this->GetOwner()->AddChild(std::move(ceilingTriggerObject));
}

void PlayerComponent::Update()
{
    m_StateMachine.Update();
}

void PlayerComponent::FixedUpdate()
{
    m_StateMachine.FixedUpdate();

    // Apply velocity
    glm::vec2 displacement = m_Velocity * Timer::GetInstance().GetFixedTimeStep(); // Displacement = velocity * time

    // Update position
    glm::vec2 newPosition = glm::vec2(GetOwner()->GetLocalPosition()) + displacement;

    // Set the new position
    GetOwner()->SetLocalPosition(newPosition);

    HandleGroundCheck(newPosition);
}

void PlayerComponent::HandleGroundCheck(const glm::vec2& newPosition)
{
    // Perform raycast downwards to check if the player is grounded
    glm::vec2 rayOrigin = newPosition;
    //rayOrigin.x += m_pCollider->GetWidth() * 0.5f;
    rayOrigin.x += m_pMainCollider->GetRadius() * 2.f * 0.5f;
    glm::vec2 rayDirection(0.0f, 1.0f); // Downwards direction
    //float rayDistance = m_pCollider->GetHeight() + 0.05f;
    float rayDistance = m_pMainCollider->GetRadius() * 2.f + 0.05f;
    RaycastResult result = Raycast(rayOrigin, rayDirection, rayDistance, CollisionComponent::ColliderType::STATIC);
    if (result.hit)
    {
        //std::cout << "raycast hit x: " << result.point.x << '\n';
        if (!m_IsJumping)
        {
            SetVerticalVelocity(5);
        }
        SetIsGrounded(true);
        //std::cout << "PLAYER IS GROUNDED: " << m_IsGrounded << '\n';
    }
    else
    {
        //std::cout << "PLAYER IS NOT GROUNDED: " << m_IsGrounded << '\n';
        // Player is not grounded
        SetIsGrounded(false);
    }
}

void PlayerComponent::LateUpdate()
{
    m_StateMachine.LateUpdate();
}