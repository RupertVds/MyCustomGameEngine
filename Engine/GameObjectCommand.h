#pragma once
#include "Command.h"
#include "GameObject.h"
#include "Timer.h"
#include <iostream>

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