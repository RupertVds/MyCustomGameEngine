#include "PlayerPointsObserver.h"
#include "PlayerPointsComponent.h"

void PlayerPointsObserver::Notify(Event event, GameObject* object)
{
	switch (event)
	{
	case Event::PLAYER_SCORE:
		auto playerPointsComponent = object->GetComponent<PlayerPointsComponent>();
		if (playerPointsComponent) m_Text->SetText("Score: " + std::to_string(playerPointsComponent->GetScore()));
		break;
	}
}
