#include "PlayerLivesObserver.h"
#include <iostream>
#include "HealthComponent.h"

PlayerLivesObserver::PlayerLivesObserver(GameObject* pOwner)
	:
	Component(pOwner)
{
	m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
}

void PlayerLivesObserver::Notify(Event event, GameObject* object)
{
	switch (event)
	{
	case Event::PLAYER_DIED:
		std::cout << "PlayerDied!\n";
		break;
	case Event::PLAYER_DAMAGE:
		auto healthComponent = object->GetComponent<HealthComponent>();
		if (healthComponent) m_pTextComponent->SetText(std::to_string(healthComponent->GetHealth()));
		break;
	}
}
