#include "PlayerComponent.h"
#include "PlayerStates.h"

PlayerComponent::PlayerComponent(GameObject* pOwner)
    :
    Component(pOwner),
    m_StateMachine(this)
{
    // Set the initial state of the player component
    m_StateMachine.SetState(new PlayerEntryState());
}

void PlayerComponent::Update()
{
    m_StateMachine.Update();
}