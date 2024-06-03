#include "BubbleComponent.h"
#include "Timer.h"
#include <AnimatorComponent.h>
#include <CircleColliderComponent.h>

BubbleComponent::BubbleComponent(GameObject* pOwner, const glm::vec2& dir)
	:
	Component(pOwner),
    m_Velocity{dir}
{
    m_pAnimator = GetOwner()->GetComponent<AnimatorComponent>();
    m_pMainTrigger = GetOwner()->GetComponent<CircleColliderComponent>();
    m_pAnimator->Play("forming", false);
}

void BubbleComponent::Render() const
{
}

void BubbleComponent::Update()
{
    switch (m_State)
    {
    case BubbleComponent::State::FORMING:
        m_FormingTimer += Timer::GetInstance().GetDeltaTime();

        if (m_FormingTimer >= m_FormingTime)
        {
            m_FormingTimer = 0;
            m_pAnimator->Play("complete");
            m_State = State::COMPLETE;
        }
        break;
    case BubbleComponent::State::COMPLETE:
        m_PoppingTimer += Timer::GetInstance().GetDeltaTime();

        if (m_PoppingTimer >= m_PoppingTime)
        {
            m_PoppingTimer = 0;
            GetOwner()->DeleteSelf();
        }
        break;
    }

    if (m_pMainTrigger->IsTriggered())
    {
        auto triggeredObjects = m_pMainTrigger->GetTriggeredObjects();
        for (auto triggeredObject : triggeredObjects)
        {
            if (triggeredObject->GetName() == "player_2")
            {
                GetOwner()->DeleteSelf();
            }
        }
    }
}

void BubbleComponent::FixedUpdate()
{
    switch (m_State)
    {
    case BubbleComponent::State::FORMING:
        // Apply velocity
        glm::vec2 displacement = m_Velocity * m_Speed * Timer::GetInstance().GetFixedTimeStep(); // Displacement = velocity * time

        // Update position
        glm::vec2 newPosition = glm::vec2(GetOwner()->GetWorldPosition()) + displacement;

        // Set the new position
        GetOwner()->SetLocalPosition(newPosition);
        break;
    case BubbleComponent::State::COMPLETE:
        break;
    }
}
