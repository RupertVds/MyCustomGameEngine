#pragma once
#include "Observer.h"
#include "SteamAchievements.h"

class Achievements final : public Observer
{
public:
	Achievements() : Observer() {}
	~Achievements() = default;
	Achievements(const Achievements& other) = delete;
	Achievements& operator=(const Achievements& other) = delete;
	Achievements(Achievements&& other) = delete;
	Achievements& operator=(Achievements&& other) = delete;
public:
	void Notify(Event event, GameObject* object) override;
private:
	bool m_Completed_ACH_WIN_100_GAMES{};
	bool m_Completed_ACH_WIN_ONE_GAME{};
};

// Achievement array which will hold data about the achievements and their state
extern Achievement_t g_Achievements[];
// Global access to Achievements object
extern CSteamAchievements* g_SteamAchievements;

