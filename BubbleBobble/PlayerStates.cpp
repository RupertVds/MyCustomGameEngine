#include "PlayerStates.h"
#include "Timer.h"
#include <Utils.h>

class BoxColliderComponent;

// Definition of methods for PlayerEntryState
void PlayerEntryState::Entry(BehaviorStateMachine<PlayerComponent>& stateMachine) 
{
    std::cout << "PlayerEntryState: Entered" << '\n';
    PlayerComponent* playerComp = stateMachine.GetComponent();

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
void PlayerAliveState::Entry(BehaviorStateMachine<PlayerComponent>&)
{
    std::cout << "PlayerAliveState: Entered" << std::endl;
    m_MoveSpeed = 70.f;
    m_JumpSpeed = 60.f;
    m_JumpTime = 2.f;
    m_JumpTimeThreshold = m_JumpTime * 0.8f;
    m_FallingSpeed = 30.f;
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

    // Check if the player is jumping
    if (playerComp->IsJumping())
    {
        float velocity{};
        (m_ReachedTopOfJump) ? velocity = m_JumpSpeed : velocity = -m_JumpSpeed;

        // Handle jumping logic
        if (m_JumpTimer < m_JumpTimeThreshold) // Below the threshold
        {
            playerComp->SetVerticalVelocity(velocity);
        }
        else if (m_JumpTimer < m_JumpTime) // Between threshold and full jump time
        {
            // Set velocity to half of the inverse of the original jump speed
            playerComp->SetVerticalVelocity(velocity * 0.5f);
        }
        else // Reached the top of the jump
        {
            if (!m_ReachedTopOfJump)
            {
                m_ReachedTopOfJump = true;
                m_JumpTimer -= m_JumpTime;
            }
        }

        // Increment jump timer
        m_JumpTimer += Timer::GetInstance().GetDeltaTime();

        // Check if jump time has elapsed
        if (m_JumpTimer >= m_JumpTime && m_ReachedTopOfJump)
        {
            // Reset jump state
            playerComp->SetIsJumping(false);
            m_JumpTimer -= m_JumpTime;
            m_ReachedTopOfJump = false;
        }
    }

    if (!playerComp->IsJumping() && !playerComp->IsGrounded())
    {
        playerComp->SetVerticalVelocity(20.f);
    }

    // Perform raycast upwards to check if the player is hitting a ceiling
    glm::vec2 rayOrigin = playerComp->GetPosition();
    rayOrigin.x += playerComp->GetCollider()->GetWidth() *0.5f;
    glm::vec2 rayDirection(0.0f, -1.0f); // Upwards direction
    float rayDistance = 0.05f;
    RaycastResult result = Raycast(rayOrigin, rayDirection, rayDistance, CollisionComponent::ColliderType::STATIC);
    if (result.hit)
    {
        std::cout << "CEILING HIT\n";
        playerComp->SetVerticalVelocity(10);
        m_JumpTimer = 0;
        m_ReachedTopOfJump = false;
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