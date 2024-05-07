#pragma once
#include "Component.h"

class PlayerComponent final : Component
{
public:
	PlayerComponent(GameObject* pOwner) : Component(pOwner) {}
	~PlayerComponent() = default;
	PlayerComponent(const PlayerComponent& other) = delete;
	PlayerComponent& operator=(const PlayerComponent& other) = delete;
	PlayerComponent(PlayerComponent&& other) = delete;
	PlayerComponent& operator=(PlayerComponent&& other) = delete;
public:
	virtual void Update() override;

private:
};

