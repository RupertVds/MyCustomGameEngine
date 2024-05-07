#pragma once
#include "Component.h"
#include "BehaviorStateMachine.h"
#include "BehaviorState.h"
#include "box2d\box2d.h"

class PlayerComponent final : public Component
{
public:
	PlayerComponent(GameObject* pOwner);
	~PlayerComponent() = default;
	PlayerComponent(const PlayerComponent& other) = delete;
	PlayerComponent& operator=(const PlayerComponent& other) = delete;
	PlayerComponent(PlayerComponent&& other) = delete;
	PlayerComponent& operator=(PlayerComponent&& other) = delete;

public:
	virtual void Update() override;

private:
	BehaviorStateMachine<PlayerComponent> m_StateMachine;
	b2Body* m_pBody{};
	b2Vec2 m_Velocity{};
	b2Vec2 m_Acceleration{};
	bool m_IsGrounded{};
};

