#include "Achievements.h"
#include <iostream>
#include "PlayerPointsComponent.h"

void Achievements::Notify(Event event, [[maybe_unused]] GameObject* object)
{
	switch (event)
	{
	case Event::IncreasedScore:
		auto playerPointsComponent = object->GetComponent<PlayerPointsComponent>();

		if (playerPointsComponent->GetScore() >= 500 && !m_Completed_ACH_WIN_ONE_GAME)
		{
			std::cout << "UNLOCKED ACH_WIN_ONE_GAMES ACHIEVEMENT\n";
			if (g_SteamAchievements)
				g_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
			m_Completed_ACH_WIN_ONE_GAME = true;
		}
		if (playerPointsComponent->GetScore() >= 1000 && !m_Completed_ACH_WIN_100_GAMES)
		{
			std::cout << "UNLOCKED ACH_WIN_100_GAMES ACHIEVEMENT\n";
			if (g_SteamAchievements)
				g_SteamAchievements->SetAchievement("ACH_WIN_100_GAMES");
			m_Completed_ACH_WIN_100_GAMES = true;
		}
	}
}

Achievement_t g_Achievements[] =
{
	_ACH_ID(ACH_WIN_ONE_GAME, "Winner"),
	_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
	_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"),
	_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter"),
};

CSteamAchievements* g_SteamAchievements{ nullptr };