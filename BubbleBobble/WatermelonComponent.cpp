#include "WatermelonComponent.h"
#include "BoxColliderComponent.h"

WatermelonComponent::WatermelonComponent(GameObject* pOwner)
	:
	Component(pOwner)
{
	m_pTrigger = this->GetOwner()->GetComponent<BoxColliderComponent>();
}

void WatermelonComponent::Render() const
{
}

void WatermelonComponent::Update()
{
}

void WatermelonComponent::FixedUpdate()
{
    if (m_pTrigger->IsTriggered())
    {
        auto triggeredObjects = m_pTrigger->GetTriggeredObjects();
        for (auto triggeredObject : triggeredObjects)
        {
            if ((triggeredObject->GetName() == "player_1" || triggeredObject->GetName() == "player_2"))
            {
                GetOwner()->DeleteSelf();
                // we could assume this is always safe
                auto playerComp = triggeredObject->GetComponent<PlayerComponent>();
                if (playerComp)
                {
                    playerComp->GetOwner()->NotifyObservers(Event::WATERMELON_PICKUP);
                    playerComp->AddScore(m_ScoreGain);
                }
            }
        }
    }
}
