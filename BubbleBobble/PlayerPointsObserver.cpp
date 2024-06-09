#include "PlayerPointsObserver.h"
#include "PlayerComponent.h"

PlayerPointsObserver::PlayerPointsObserver(GameObject* pOwner)
	:
	Component(pOwner)
{
	m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
}

void PlayerPointsObserver::Notify(Event event, GameObject* object)
{
	switch (event)
	{
	case Event::PLAYER_SCORE:
		auto playerComp = object->GetComponent<PlayerComponent>();
		if (playerComp) m_pTextComponent->SetText(std::to_string(playerComp->GetScore()));
		break;
	}
}
