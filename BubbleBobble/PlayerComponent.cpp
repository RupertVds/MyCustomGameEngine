#include "PlayerComponent.h"
#include "PlayerStates.h"
#include <Timer.h>
#include <Renderer.h>
#include <Utils.h>
#include <AnimatorComponent.h>

PlayerComponent::PlayerComponent(GameObject* pOwner)
    :
    Component(pOwner),
    m_StateMachine(this)
{
    // Set the initial state of the player component
    m_StateMachine.SetState(new PlayerEntryState());
    m_pMainCollider = GetOwner()->GetComponent<BoxColliderComponent>();
    m_pAnimator = GetOwner()->GetComponent<AnimatorComponent>();

    auto ceilingTriggerObject = std::make_unique<GameObject>();
    m_pCeilingTrigger = ceilingTriggerObject->AddComponent<BoxColliderComponent>(m_pMainCollider->GetWidth() - 2.f, 12.f, CollisionComponent::ColliderType::STATIC, true);
    ceilingTriggerObject->SetLocalPosition({ m_pMainCollider->GetWidth() / 2 - m_pCeilingTrigger->GetWidth() / 2, -10.f});
    this->GetOwner()->AddChild(std::move(ceilingTriggerObject));

    auto jumpingCorrectionTriggerObject = std::make_unique<GameObject>();
    m_pJumpCorrectionTrigger = jumpingCorrectionTriggerObject->AddComponent<BoxColliderComponent>(m_pMainCollider->GetWidth(), m_pMainCollider->GetHeight(), CollisionComponent::ColliderType::STATIC, true);
    jumpingCorrectionTriggerObject->SetLocalPosition({ 0, 0});
    this->GetOwner()->AddChild(std::move(jumpingCorrectionTriggerObject));
}

void PlayerComponent::Update()
{
    m_StateMachine.Update();
}

void PlayerComponent::FixedUpdate()
{
    m_StateMachine.FixedUpdate();

    // Apply velocity
    glm::vec2 displacement = m_Velocity * Timer::GetInstance().GetFixedTimeStep(); // Displacement = velocity * time

    // Update position
    glm::vec2 newPosition = glm::vec2(GetOwner()->GetWorldPosition()) + displacement;

    // Set the new position
    GetOwner()->SetLocalPosition(newPosition);
}

void PlayerComponent::LateUpdate()
{
    m_StateMachine.LateUpdate();
}