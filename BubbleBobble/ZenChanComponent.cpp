#include "ZenChanComponent.h"
#include "ZenChanStates.h"
#include <AnimatorComponent.h>
#include <BoxColliderComponent.h>

ZenChanComponent::ZenChanComponent(GameObject* pOwner)
	:
	Component(pOwner),
	m_StateMachine(this)
{
	m_pAnimator = GetOwner()->GetComponent<AnimatorComponent>();

	m_pMainCollider = GetOwner()->GetComponent<BoxColliderComponent>();

	auto jumpingCorrectionTriggerObject = std::make_unique<GameObject>();
	m_pJumpCorrectionTrigger = jumpingCorrectionTriggerObject->AddComponent<BoxColliderComponent>(m_pMainCollider->GetWidth(), m_pMainCollider->GetHeight(), CollisionComponent::ColliderType::STATIC, true);
	jumpingCorrectionTriggerObject->SetLocalPosition({ 0, 0 });
	jumpingCorrectionTriggerObject->SetLocalPosition(jumpingCorrectionTriggerObject->GetLocalPosition() + m_pMainCollider->GetOffset());

	m_StateMachine.SetState(new ZenChanEntryState());

}

void ZenChanComponent::Update()
{
	m_StateMachine.Update();
}

void ZenChanComponent::FixedUpdate()
{
	m_StateMachine.FixedUpdate();
}

void ZenChanComponent::LateUpdate()
{
	m_StateMachine.LateUpdate();
}
