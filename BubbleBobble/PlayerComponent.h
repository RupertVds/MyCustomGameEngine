#pragma once
#include "Component.h"
#include "BehaviorStateMachine.h"
#include "BehaviorState.h"

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
	void SetIsGrounded(bool isGrounded) { m_IsGrounded = isGrounded; }
	bool IsGrounded() { return m_IsGrounded; }

	const glm::vec2& GetMovingDirection() { return m_MovementDirection; }
	void SetMovingDirection(const glm::vec2& direction) { m_MovementDirection = direction; }

	void SetVelocity(const glm::vec2& velocity) { m_Velocity = velocity; }
	void SetHorizontalVelocity(float norm) { m_Velocity.x = norm; }
	void SetVerticalVelocity(float norm) { m_Velocity.y = norm; }
	const glm::vec2& GetVelocity() const { return m_Velocity; }

	void SetAcceleration(const glm::vec2& acceleration) { m_Acceleration = acceleration; }
	void SetHorizontalAcceleration(float norm) { m_Acceleration.x = norm; }
	void SetVerticalAcceleration(float norm) { m_Acceleration.y = norm; }
	const glm::vec2& GetAcceleration() const { return m_Acceleration; }
private:
	BehaviorStateMachine<PlayerComponent> m_StateMachine;
	bool m_IsGrounded{};
	glm::vec2 m_Velocity{};
	glm::vec2 m_Acceleration{0.f, 9.81f};
	glm::vec2 m_MovementDirection{};
};