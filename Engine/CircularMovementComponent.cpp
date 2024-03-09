#include "CircularMovementComponent.h"
#include "Timer.h"
#include <iostream>

CircularMovementComponent::CircularMovementComponent(GameObject* pOwner, float radius, float speed, const glm::vec3& rootPosition)
    : Component{pOwner},
    m_Radius(radius), m_Speed(speed), m_Angle(0.0f), m_RootPosition(rootPosition) 
{}

void CircularMovementComponent::Update()
{
    // Update the angle based on time or other factors
    m_Angle += Timer::GetInstance().GetDeltaTime() * m_Speed;

    // Calculate the new position in a circular path around the root position
    float x = m_RootPosition.x + m_Radius * std::cos(m_Angle);
    float y = m_RootPosition.y + m_Radius * std::sin(m_Angle);

    // Set the local position of the GameObject
    GetOwner()->SetLocalPosition(glm::vec3(x, y, m_RootPosition.z));
}