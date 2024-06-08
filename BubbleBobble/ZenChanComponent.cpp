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

	auto mainTriggerObject = std::make_unique<GameObject>();
	m_pMainTrigger = mainTriggerObject->AddComponent<BoxColliderComponent>(m_pMainCollider->GetWidth(), m_pMainCollider->GetHeight(), CollisionComponent::ColliderType::DYNAMIC, true, CollisionComponent::ColliderType::DYNAMIC);
	mainTriggerObject->SetLocalPosition({ 5, 0 });
	mainTriggerObject->SetLocalPosition(mainTriggerObject->GetLocalPosition() + m_pMainCollider->GetOffset());
	this->GetOwner()->AddChild(std::move(mainTriggerObject));
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