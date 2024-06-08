#include "BubbleComponent.h"
#include "Timer.h"
#include <AnimatorComponent.h>
#include <CircleColliderComponent.h>
#include <cmath>
#include "PlayerStates.h"
#include "ZenChanComponent.h"
#include "ZenChanStates.h"

BubbleComponent::BubbleComponent(GameObject* pOwner, const glm::vec2& dir, PlayerComponent* owner)
    : Component(pOwner),
    m_Velocity{ dir },
    m_pPlayerOwner{owner},
    m_Amplitude{ 0.5f },   // Amplitude of the sine wave
    m_Frequency{ 2.0f }    // Frequency of the sine wave
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

            m_Velocity = { 0, -1 };
            m_State = State::COMPLETE;
        }

        // while forming, we can hit enemies
        if (m_pMainTrigger->IsTriggered())
        {
            auto triggeredObjects = m_pMainTrigger->GetTriggeredObjects();
            for (auto triggeredObject : triggeredObjects)
            {
                if ((triggeredObject->GetName() == "player_1" || triggeredObject->GetName() == "player_2") && triggeredObject != m_pPlayerOwner->GetOwner())
                {
                    GetOwner()->DeleteSelf();
                    // we could assume this is always safe
                    auto playerComp = triggeredObject->GetComponent<PlayerComponent>();
                    if (playerComp)
                    {
                        playerComp->GetStateMachine()->SetState(new PlayerDeadState());
                    }
                }

                if (triggeredObject)
                {
                    auto zenChanObject = triggeredObject->GetComponent<ZenChanComponent>();
                    if (zenChanObject)
                    {
                        // TODO: replace with going in death state
                        zenChanObject->GetStateMachine()->SetState(new ZenChanDeadState());

                        this->GetOwner()->DeleteSelf();
                    }
                }
            }
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


}

void BubbleComponent::FixedUpdate()
{
    glm::vec2 newPosition = glm::vec2(GetOwner()->GetWorldPosition());

    switch (m_State)
    {
    case BubbleComponent::State::FORMING:
        // Apply velocity
        newPosition += m_Velocity * m_FormingSpeed * Timer::GetInstance().GetFixedTimeStep(); // Displacement = velocity * time
        GetOwner()->SetLocalPosition(newPosition);
        break;
    case BubbleComponent::State::COMPLETE:
    {
        // Apply an upward movement and horizontal sine wave motion
        float elapsedTime = Timer::GetInstance().GetTotalTime();
        float horizontalDisplacement = m_Amplitude * sin(m_Frequency * elapsedTime);
        glm::vec2 sinWavePosition = newPosition + glm::vec2(horizontalDisplacement, -m_CompletedSpeed * Timer::GetInstance().GetFixedTimeStep());

        // Update position with sine wave effect
        GetOwner()->SetLocalPosition(sinWavePosition);
    }
    break;
    }
}
