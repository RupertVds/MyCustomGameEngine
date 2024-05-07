#include "PlayerStates.h"

// Definition of methods for PlayerEntryState
void PlayerEntryState::Entry(BehaviorStateMachine<PlayerComponent>&) 
{
    std::cout << "PlayerEntryState: Entered" << std::endl;
}

void PlayerEntryState::Update(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    std::cout << "PlayerEntryState: Updated" << std::endl;
    stateMachine.SetState(new PlayerAliveState());
}

void PlayerEntryState::Exit(BehaviorStateMachine<PlayerComponent>&) 
{
    std::cout << "PlayerEntryState: Exited" << std::endl;
}

// Definition of methods for PlayerAliveState
void PlayerAliveState::Entry(BehaviorStateMachine<PlayerComponent>&)
{
    std::cout << "PlayerAliveState: Entered" << std::endl;
}

void PlayerAliveState::Update(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    std::cout << "PlayerAliveState: Updated" << std::endl;
    stateMachine.SetState(new PlayerDeadState());
}

void PlayerAliveState::Exit(BehaviorStateMachine<PlayerComponent>&)
{
    std::cout << "PlayerAliveState: Exited" << std::endl;
}

// Definition of methods for PlayerDeadState
void PlayerDeadState::Entry(BehaviorStateMachine<PlayerComponent>&)
{
    std::cout << "PlayerDeadState: Entered" << std::endl;
}

void PlayerDeadState::Update(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    std::cout << "PlayerDeadState: Updated" << std::endl;
    stateMachine.SetState(new PlayerEntryState());
}

void PlayerDeadState::Exit(BehaviorStateMachine<PlayerComponent>&) 
{
    std::cout << "PlayerDeadState: Exited" << std::endl;
}