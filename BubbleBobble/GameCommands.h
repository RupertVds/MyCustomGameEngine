#pragma once
#include "GameObjectCommand.h"

//class MoveHorizontalCommand : public GameObjectCommand
//{
//public:
//	MoveHorizontalCommand(GameObject* object, const glm::vec2& direction) : MoveHorizontalCommand(object), m_MovementComponent(object->GetComponent<PlayerMovementComponent>()) {}
//
//	virtual void Execute() override
//	{
//		if (m_MovementComponent)
//		{
//			m_MovementComponent->MoveLeft();
//		}
//	}
//
//private:
//	PlayerMovementComponent* m_MovementComponent;
//};

class MoveLeftCommand : public GameObjectCommand
{
public:
	MoveLeftCommand(GameObject* object) : GameObjectCommand(object), m_MovementComponent(object->GetComponent<PlayerMovementComponent>()) {}

	virtual void Execute() override
	{
		if (m_MovementComponent)
		{
			m_MovementComponent->MoveLeft();
		}
	}

private:
	PlayerMovementComponent* m_MovementComponent;
};

class MoveRightCommand : public GameObjectCommand
{
public:
	MoveRightCommand(GameObject* object) : GameObjectCommand(object), m_MovementComponent(object->GetComponent<PlayerMovementComponent>()) {}

	virtual void Execute() override
	{
		if (m_MovementComponent)
		{
			m_MovementComponent->MoveRight();
		}
	}

private:
	PlayerMovementComponent* m_MovementComponent;
};

class JumpCommand : public GameObjectCommand
{
public:
	JumpCommand(GameObject* object) : GameObjectCommand(object), m_MovementComponent(object->GetComponent<PlayerMovementComponent>()) {}

	virtual void Execute() override
	{
		if (m_MovementComponent)
		{
			m_MovementComponent->Jump();
		}
	}

private:
	PlayerMovementComponent* m_MovementComponent;
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