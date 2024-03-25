#include "PlayerLivesObserver.h"
#include <iostream>
#include "HealthComponent.h"

void PlayerLivesObserver::Notify(Event event, [[maybe_unused]] GameObject* object)
{
	switch (event)
	{
	case Event::ActorDied:
		std::cout << "PlayerDied!\n";
		break;
	case Event::ActorDamaged:
		auto healthComponent = object->GetComponent<HealthComponent>();
		if (healthComponent) m_Text->SetText("# lives: " + std::to_string(healthComponent->GetHealth()));
		break;
	}
}
