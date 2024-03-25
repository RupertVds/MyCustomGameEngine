#pragma once
#include "Component.h"

class PlayerPointsComponent final : public Component
{
public:
	PlayerPointsComponent(GameObject* pOwner) :
		Component(pOwner)
	{}
	~PlayerPointsComponent() = default;
	PlayerPointsComponent(const PlayerPointsComponent& other) = delete;
	PlayerPointsComponent& operator=(const PlayerPointsComponent& other) = delete;
	PlayerPointsComponent(PlayerPointsComponent&& other) = delete;
	PlayerPointsComponent& operator=(PlayerPointsComponent&& other) = delete;
public:
	void AddScore(int amount)
	{
		m_Score += amount;
		GetOwner()->NotifyObservers(Event::IncreasedScore);
	}

	unsigned int GetScore() const { return m_Score; }
private:
	unsigned int m_Score{};
};