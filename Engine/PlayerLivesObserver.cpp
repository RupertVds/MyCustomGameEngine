#include "PlayerLivesObserver.h"
#include <iostream>
#include "HealthComponent.h"

void PlayerLivesObserver::Notify(Event event, GameObject* object)
{
	switch (event)
	{
	case Event::PLAYER_DIED:
		std::cout << "PlayerDied!\n";
		break;
	case Event::PLAYER_DAMAGE:
		auto healthComponent = object->GetComponent<HealthComponent>();
		if (healthComponent) m_Text->SetText("# lives: " + std::to_string(healthComponent->GetHealth()));
		break;
	}
}
