#pragma once
#include "Component.h"
#include "BehaviorStateMachine.h"
#include "BehaviorState.h"
#include "box2d\box2d.h"
#include "PhysicsComponent.h"

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
	virtual void FixedUpdate() override;
	virtual void LateUpdate() override;
	inline PhysicsComponent* GetPhysicsComponent() { return m_pPhysicsComponent; }
	inline b2Body* GetBody() { return m_pBody; }
	void SetIsGrounded(bool isGrounded) { m_IsGrounded = isGrounded; }
	bool IsGrounded() { return m_IsGrounded; }

	const b2Vec2& GetMovingDirection() { return m_MovementDirection; }
	void SetMovingDirection(const b2Vec2& direction) { m_MovementDirection = direction; }
private:
	BehaviorStateMachine<PlayerComponent> m_StateMachine;
	PhysicsComponent* m_pPhysicsComponent{};
	b2Body* m_pBody{};
	bool m_IsGrounded{};
	b2Vec2 m_MovementDirection{};
};

