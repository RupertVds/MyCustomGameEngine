#pragma once
#include "Component.h"
#include "BehaviorStateMachine.h"
#include "BehaviorState.h"

class BoxColliderComponent;
class CircleColliderComponent;

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

	void SetIsJumping(bool state) { m_IsJumping = state; }
	void Jump() 
	{ 
		if (m_IsGrounded)
		{
			m_IsJumping = true;
		}
	}
	bool IsJumping() { return m_IsJumping; }

	const glm::vec2& GetMovingDirection() { return m_MovementDirection; }
	void SetMovingDirection(const glm::vec2& direction) { m_MovementDirection = direction; }

	void SetVelocity(const glm::vec2& velocity) { m_Velocity = velocity; }
	void SetHorizontalVelocity(float norm) { m_Velocity.x = norm; }
	void SetVerticalVelocity(float norm) { m_Velocity.y = norm; }
	const glm::vec2& GetVelocity() const { return m_Velocity; }
	glm::vec2 GetPosition() const { return GetOwner()->GetLocalPosition(); }
	BoxColliderComponent* GetCollider() const { return m_pMainCollider; }
	BoxColliderComponent* GetCeilingTrigger() const { return m_pCeilingTrigger; }
	BoxColliderComponent* GetJumpCorrectionTrigger() const { return m_pJumpCorrectionTrigger; }
private:
	BehaviorStateMachine<PlayerComponent> m_StateMachine;
	BoxColliderComponent* m_pMainCollider{};

	BoxColliderComponent* m_pCeilingTrigger{};
	BoxColliderComponent* m_pGroundTrigger{};
	BoxColliderComponent* m_pJumpCorrectionTrigger{};

	bool m_IsGrounded{};
	bool m_IsJumping{};
	glm::vec2 m_Velocity{};
	glm::vec2 m_MovementDirection{};
};