#pragma once
#include "GameObjectCommand.h"
#include "PlayerComponent.h"
#include "PlayerMovementComponent.h"
#include "HealthComponent.h"
#include "PlayerPointsComponent.h"

class MoveHorizontalCommand : public GameObjectCommand
{
public:
	MoveHorizontalCommand(GameObject* object, const glm::vec2& direction) 
		: GameObjectCommand(object), 
		m_PlayerComponent(object->GetComponent<PlayerComponent>()),
		m_Direction{direction}
	{}

	virtual void Execute() override
	{
		if (m_PlayerComponent)
		{
			// If the movement direction was already set by a different command
			// that means we are trying to move left and right
			// this means that we should have no horizontal move dir
			// -> the player component resets this direction every late update
			if (m_PlayerComponent->GetMovingDirection().x > 0)
			{
				m_PlayerComponent->SetMovingDirection({ 0.f, 0.f });
			}
			else if (m_PlayerComponent->GetMovingDirection().x < 0)
			{
				m_PlayerComponent->SetMovingDirection({ 0.f, 0.f });
			}
			else
			{
				m_PlayerComponent->SetMovingDirection({ m_Direction.x, m_Direction.y });
			}
		}
	}

private:
	PlayerComponent* m_PlayerComponent;
	glm::vec2 m_Direction{};
};

class JumpCommand : public GameObjectCommand
{
public:
	JumpCommand(GameObject* object) : GameObjectCommand(object), m_PlayerComponent(object->GetComponent<PlayerComponent>()) {}

	virtual void Execute() override
	{
		if (m_PlayerComponent)
		{
			m_PlayerComponent->Jump();
		}
	}

private:
	PlayerComponent* m_PlayerComponent;
};

class PlayerAttackCommand : public GameObjectCommand
{
public:
	PlayerAttackCommand(GameObject* object) : GameObjectCommand(object), m_PlayerComponent(object->GetComponent<PlayerComponent>()) {}

	virtual void Execute() override
	{
		if (m_PlayerComponent)
		{
			m_PlayerComponent->SetIsAttacking(true);
		}
	}

private:
	PlayerComponent* m_PlayerComponent;
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
		m_Id{ id }, m_Volume{ volume }
	{}

	virtual void Execute() override
	{
		ServiceLocator::GetSoundSystem().Play(m_Id, m_Volume);
	}

private:
	const std::string m_Id{};
	const float m_Volume{};
};