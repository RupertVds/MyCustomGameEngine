#pragma once
#include "Component.h"
#include "BehaviorStateMachine.h"
#include "BehaviorState.h"
#include <Scene.h>

class BoxColliderComponent;
class CircleColliderComponent;
class AnimatorComponent;

class ZenChanComponent final : public Component
{
public:
	ZenChanComponent(GameObject* pOwner);
	~ZenChanComponent() = default;
	ZenChanComponent(const ZenChanComponent& other) = delete;
	ZenChanComponent& operator=(const ZenChanComponent& other) = delete;
	ZenChanComponent(ZenChanComponent&& other) = delete;
	ZenChanComponent& operator=(ZenChanComponent&& other) = delete;

public:
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void LateUpdate() override;

	void SetIsGrounded(bool isGrounded) { m_IsGrounded = isGrounded; }
	bool IsGrounded() { return m_IsGrounded; }

	void SetVelocity(const glm::vec2& velocity) { m_Velocity = velocity; }
	void SetHorizontalVelocity(float norm) { m_Velocity.x = norm; }
	void SetVerticalVelocity(float norm) { m_Velocity.y = norm; }
	const glm::vec2& GetVelocity() const { return m_Velocity; }
	glm::vec2 GetPosition() const { return GetOwner()->GetLocalPosition(); }
	BoxColliderComponent* GetCollider() const { return m_pMainCollider; }
	BoxColliderComponent* GetTrigger() const { return m_pMainTrigger; }
	AnimatorComponent* GetAnimator() { return m_pAnimator; }
	BehaviorStateMachine<ZenChanComponent>* GetStateMachine() { return &m_StateMachine; }
private:
	BehaviorStateMachine<ZenChanComponent> m_StateMachine;
	AnimatorComponent* m_pAnimator{};
	BoxColliderComponent* m_pMainCollider{};
	BoxColliderComponent* m_pMainTrigger{};

	bool m_IsGrounded{};

	glm::vec2 m_Velocity{};
};

