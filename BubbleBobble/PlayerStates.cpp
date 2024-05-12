#include "PlayerStates.h"
#include "Timer.h"

// Definition of methods for PlayerEntryState
void PlayerEntryState::Entry(BehaviorStateMachine<PlayerComponent>& stateMachine) 
{
    std::cout << "PlayerEntryState: Entered" << '\n';
    PlayerComponent* playerComp = stateMachine.GetComponent();

    // set no acceleration
    playerComp->SetAcceleration({});
    // set to constant velocity
    playerComp->SetVelocity({ 0, 10.f });
}

void PlayerEntryState::Update(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    std::cout << "PlayerEntryState: Updated" << std::endl;
    m_MovingDownElapsedTime += Timer::GetInstance().GetDeltaTime();
    if (m_MovingDownElapsedTime >= m_MovingDownTime)
    {
        stateMachine.SetState(new PlayerAliveState());
    }
}

void PlayerEntryState::Exit(BehaviorStateMachine<PlayerComponent>&) 
{
    std::cout << "PlayerEntryState: Exited" << std::endl;
}

// Definition of methods for PlayerAliveState
void PlayerAliveState::Entry(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    std::cout << "PlayerAliveState: Entered" << std::endl;
    m_MoveSpeed = 70.f;
    PlayerComponent* playerComp = stateMachine.GetComponent();
    playerComp->SetAcceleration({0, 9.81f});

}

void PlayerAliveState::Update(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    PlayerComponent* playerComp = stateMachine.GetComponent();
    glm::vec2 moveDir = stateMachine.GetComponent()->GetMovingDirection();
    if (moveDir.x > 0)
    {
        playerComp->SetHorizontalVelocity(m_MoveSpeed);
    }
    else if (moveDir.x < 0)
    {
        playerComp->SetHorizontalVelocity(-m_MoveSpeed);
    }
    else if (moveDir.x == 0)
    {
        playerComp->SetHorizontalVelocity(0);
    }

    if (playerComp->IsGrounded() && playerComp->IsJumping())
    {
        playerComp->SetVerticalVelocity(-30.f);
        playerComp->SetIsJumping(false);
    }
}

void PlayerAliveState::LateUpdate(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    stateMachine.GetComponent()->SetMovingDirection({ 0, 0 });
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