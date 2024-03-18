#pragma once
#include "Command.h"
#include "GameObject.h"
#include "Timer.h"
#include <iostream>
#include "PlayerMovementComponent.h"

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