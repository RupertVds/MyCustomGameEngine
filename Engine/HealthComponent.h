#pragma once
#include "Component.h"

class HealthComponent final : public Component
{
public:
	HealthComponent(GameObject* pOwner, int maxHealth = 3) : 
		Component(pOwner), 
		m_MaxHealth{ maxHealth }, 
		m_Health{ maxHealth } 
	{

	}
	~HealthComponent() = default;
	HealthComponent(const HealthComponent& other) = delete;
	HealthComponent& operator=(const HealthComponent& other) = delete;
	HealthComponent(HealthComponent&& other) = delete;
	HealthComponent& operator=(HealthComponent&& other) = delete;
public:
	void TakeDamage(int amount)
	{
		m_Health -= amount;
		GetOwner()->NotifyObservers(Event::ActorDamaged);
		if (m_Health == 0)
		{
			GetOwner()->NotifyObservers(Event::ActorDied);
		}
	}

	void Heal(int amount)
	{
		m_Health += amount;
	}

	bool IsDead() const
	{
		return (m_Health <= 0) ? true : false;
	}

	int GetHealth() const { return m_Health; }
private:
	int m_Health{};
	int m_MaxHealth{};
};

