#pragma once
#include "Component.h"
#include "Timer.h"
#include "EventQueue.h"
#include "HealthComponent.h"

class PlayerMovementComponent final : public Component
{
public:
    PlayerMovementComponent(GameObject* pOwner, float maxSpeed) : Component(pOwner),
        m_MaxSpeed{ maxSpeed }
    {
        //EventQueue::GetInstance().AttachEvent(Event::PLAYER_DAMAGE, std::bind(&PlayerMovementComponent::TestEventQueue, this));
    }

    virtual void Update() override
    {
        glm::vec3 newPosition = GetOwner()->GetLocalPosition();

        // Normalize the direction vector
        float length = sqrt(m_Direction.x * m_Direction.x + m_Direction.y * m_Direction.y);
        if (length > 0)
        {
            m_Direction.x = m_Direction.x / length;
            m_Direction.y = m_Direction.y / length;
        }
        newPosition.x += m_Direction.x * m_MaxSpeed * Timer::GetInstance().GetDeltaTime();
        newPosition.y += m_Direction.y * m_MaxSpeed * Timer::GetInstance().GetDeltaTime();
        GetOwner()->SetLocalPosition(newPosition);
        m_Direction = glm::vec2{};
    }

    // Add the direction of movement
    // Since the Process Input goes before the update calls
    // This will be a good way to handle the speed boosting problem (diagonal movement is faster)
    // It will also centralize the movement and let the move commands do one thing
    // Change the direction of movement
    void AddDirection(const glm::vec2& direction)
    {
        // Add the given direction to the total direction vector
        m_Direction += direction;
    }

    //void TestEventQueue()
    //{
    //    std::cout << "TEST EVENT QUEUE FIRED IN PLAYER MOVEMENT COMP!\n";
    //    if (GetOwner()->GetComponent<HealthComponent>()->GetHealth() <= 0)
    //    {
    //        GetOwner()->DeleteSelf();

    //    }
    //}

private:
    glm::vec2 m_Direction{};
    float const m_MaxSpeed{};
};