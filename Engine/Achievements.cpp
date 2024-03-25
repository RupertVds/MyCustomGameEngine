#include "Achievements.h"
#include <iostream>

void Achievements::Notify(Event event, [[maybe_unused]] GameObject* object)
{
	switch (event)
	{
	case Event::ActorDied:
		break;
	case Event::ActorDamaged:
		break;
	case Event::ActorFell:
		break;
	default:
		break;
	}
}
