#pragma once
#include "Component.h"
#include "BehaviorStateMachine.h"
#include "BehaviorState.h"
#include <Scene.h>

class BoxColliderComponent;
class CircleColliderComponent;
class AnimatorComponent;

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

	bool IsJumping() const { return m_IsJumping; }

	void SetIsAttacking(bool state)
	{
		if (m_CanAttack)
		{
			if (state == true)
			{
				m_CanAttack = false;
			}
		}

		m_IsAttacking = state;

	}
	bool IsAttacking() const { return m_IsAttacking; }

	void SetCanAttack(bool state)
	{
		m_CanAttack = state;
	}
	bool CanAttack() const { return m_CanAttack; }

	void AddScore(int score) { m_Score += score; }
	int GetScore() const { return m_Score; }

	const glm::vec2& GetMovingDirection() { return m_MovementDirection; }
	void SetMovingDirection(const glm::vec2& direction) { m_MovementDirection = direction; }

	void SetVelocity(const glm::vec2& velocity) { m_Velocity = velocity; }
	void SetHorizontalVelocity(float norm) { m_Velocity.x = norm; }
	void SetVerticalVelocity(float norm) { m_Velocity.y = norm; }
	const glm::vec2& GetVelocity() const { return m_Velocity; }
	const glm::vec2& GetPosition() const { return GetOwner()->GetLocalPosition(); }
	BoxColliderComponent* GetCollider() const { return m_pMainCollider; }
	BoxColliderComponent* GetCeilingTrigger() const { return m_pCeilingTrigger; }
	BoxColliderComponent* GetJumpCorrectionTrigger() const { return m_pJumpCorrectionTrigger; }
	AnimatorComponent* GetAnimator() { return m_pAnimator; }
	BehaviorStateMachine<PlayerComponent>* GetStateMachine() { return &m_StateMachine; }
private:
	std::shared_ptr<Scene> m_CurrentScene;
	BehaviorStateMachine<PlayerComponent> m_StateMachine;
	AnimatorComponent* m_pAnimator{};
	BoxColliderComponent* m_pMainCollider{};

	BoxColliderComponent* m_pCeilingTrigger{};
	BoxColliderComponent* m_pJumpCorrectionTrigger{};

	int m_Score{};
	bool m_IsGrounded{};
	bool m_IsJumping{};
	bool m_IsAttacking{};
	bool m_CanAttack{ true };
	glm::vec2 m_Velocity{};
	glm::vec2 m_MovementDirection{};
};