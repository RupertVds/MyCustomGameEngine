#pragma once
#include "Command.h"
#include "GameObject.h"
#include "Timer.h"
#include <iostream>
#include "PlayerMovementComponent.h"
#include "HealthComponent.h"
#include "PlayerPointsComponent.h"
#include "ServiceLocator.h"

class GameObject;

class GameObjectCommand : public Command
{
	GameObject* m_pObject{};
protected:
	GameObject* GetGameObject() const { return m_pObject; }
public:
	explicit GameObjectCommand(GameObject* object) : 
		m_pObject{ object }
	{}
	virtual ~GameObjectCommand() = default;

	virtual void Execute() override = 0;
};

class MoveCommand final : public GameObjectCommand
{
public:
	MoveCommand(GameObject* object, const glm::vec2 direction) : GameObjectCommand(object),
		m_Direction{ direction }
	{

	}

	virtual void Execute() override
	{
		// Get the PlayerMovementComponent of the object
		PlayerMovementComponent* movementComponent = GetGameObject()->GetComponent<PlayerMovementComponent>();
		if (movementComponent)
		{
			// Add the movement vector to the total direction
			movementComponent->AddDirection(m_Direction);
		}
	}

private:
	glm::vec2 m_Direction;
};

class TakeDamageCommand final : public GameObjectCommand
{
public:
	TakeDamageCommand(GameObject* object, int amount = 1) : GameObjectCommand(object),
		m_Amount{ amount }
	{}

	virtual void Execute() override
	{
		HealthComponent* healthComponent = GetGameObject()->GetComponent<HealthComponent>();
		if (healthComponent)
		{
			healthComponent->TakeDamage(m_Amount);
		}
	}

private:
	int m_Amount{};
};

class AddScore final : public GameObjectCommand
{
public:
	AddScore(GameObject* object, unsigned int amount) : GameObjectCommand(object),
		m_Amount{ amount }
	{}

	virtual void Execute() override
	{
		PlayerPointsComponent* playerPointsComponent = GetGameObject()->GetComponent<PlayerPointsComponent>();
		if (playerPointsComponent)
		{
			playerPointsComponent->AddScore(m_Amount);
		}
	}

private:
	unsigned int m_Amount{};
};

class PlaySFX final : public GameObjectCommand
{
public:
	PlaySFX(GameObject* object, const std::string& id, float volume) : GameObjectCommand(object),
		m_Id{ id }, m_Volume{volume}
	{}

	virtual void Execute() override
	{
		ServiceLocator::get_sound_system().Play(m_Id, m_Volume);
	}

private:
	const std::string m_Id{};
	const float m_Volume{};
};