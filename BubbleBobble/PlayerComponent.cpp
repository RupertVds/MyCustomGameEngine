#include "PlayerComponent.h"
#include "PlayerStates.h"
#include <Timer.h>
#include <Renderer.h>
#include <Utils.h>
#include <AnimatorComponent.h>
#include "HealthComponent.h"

PlayerComponent::PlayerComponent(GameObject* pOwner)
    :
    Component(pOwner),
    m_StateMachine(this)
{
    // Set the initial state of the player component
    m_pAnimator = GetOwner()->GetComponent<AnimatorComponent>();
    
    m_pMainCollider = GetOwner()->GetComponent<BoxColliderComponent>();
    m_pMainCollider->SetOffset({ 5, 0 });

    auto ceilingTriggerObject = std::make_unique<GameObject>();
    m_pCeilingTrigger = ceilingTriggerObject->AddComponent<BoxColliderComponent>(m_pMainCollider->GetWidth() - 2.f, 12.f, CollisionComponent::ColliderType::STATIC, true);
    ceilingTriggerObject->SetLocalPosition({ m_pMainCollider->GetWidth() / 2 - m_pCeilingTrigger->GetWidth() / 2, -10.f});
    ceilingTriggerObject->SetLocalPosition(ceilingTriggerObject->GetLocalPosition() + m_pMainCollider->GetOffset());
    this->GetOwner()->AddChild(std::move(ceilingTriggerObject));

    auto jumpingCorrectionTriggerObject = std::make_unique<GameObject>();
    m_pJumpCorrectionTrigger = jumpingCorrectionTriggerObject->AddComponent<BoxColliderComponent>(m_pMainCollider->GetWidth(), m_pMainCollider->GetHeight(), CollisionComponent::ColliderType::STATIC, true);
    jumpingCorrectionTriggerObject->SetLocalPosition({ 0, 0});
    jumpingCorrectionTriggerObject->SetLocalPosition(jumpingCorrectionTriggerObject->GetLocalPosition() + m_pMainCollider->GetOffset());


    this->GetOwner()->AddChild(std::move(jumpingCorrectionTriggerObject));
    m_StateMachine.SetState(new PlayerEntryState());

    m_pHealthComponent = GetOwner()->GetComponent<HealthComponent>();
}

void PlayerComponent::Update()
{
    m_StateMachine.Update();
}

void PlayerComponent::FixedUpdate()
{
    m_StateMachine.FixedUpdate();
}

void PlayerComponent::LateUpdate()
{
    m_StateMachine.LateUpdate();
}