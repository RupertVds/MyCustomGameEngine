#include "PlayerStates.h"
#include "Timer.h"
#include <Utils.h>
#include <Renderer.h>

class BoxColliderComponent;

// Definition of methods for PlayerEntryState
void PlayerEntryState::Entry(BehaviorStateMachine<PlayerComponent>& stateMachine) 
{
    std::cout << "PlayerEntryState: Entered" << '\n';
    PlayerComponent* playerComp = stateMachine.GetComponent();

    // set to constant velocity
    playerComp->SetVelocity({ 0, m_MovingDownSpeed });
}

void PlayerEntryState::Update(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    //std::cout << "PlayerEntryState: Updated" << std::endl;
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
    m_MoveSpeed = 120.f;
    m_JumpSpeed = 245.f;
    m_JumpTime = 0.35f;
    m_JumpTimeThreshold = m_JumpTime * 0.9f;
    m_FallingSpeed = 140.f;
}

void PlayerAliveState::Update(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    PlayerComponent* playerComp = stateMachine.GetComponent();
    HandleHorizontalMovement(playerComp);
    HandleJumping(playerComp);

    if (!playerComp->IsJumping() && !playerComp->IsGrounded())
    {
        //stateMachine.GetComponent()->GetCollider()->SetVerticalCorrection(true);
        playerComp->SetVerticalVelocity(m_FallingSpeed);
    }
}

void PlayerAliveState::HandleJumping(PlayerComponent* playerComp)
{
    // Check if the player is jumping
    if (playerComp->IsJumping())
    {
        // Handle jumping logic
        if (m_JumpTimer < m_JumpTimeThreshold) // Below the threshold
        {
            playerComp->SetVerticalVelocity(-m_JumpSpeed);
        }
        else if (m_JumpTimer < m_JumpTime) // Between threshold and full jump time
        {
            // Set velocity to half of the inverse of the original jump speed
            playerComp->SetVerticalVelocity(-m_JumpSpeed * 0.5f);
        }
        
        // Increment jump timer
        m_JumpTimer += Timer::GetInstance().GetDeltaTime();

        // Check if jump time has elapsed
        if (m_JumpTimer >= m_JumpTime && !playerComp->GetJumpCorrectionTrigger()->IsTriggered())
        {
            // Reset jump state
            playerComp->SetIsJumping(false);
            m_JumpTimer = 0;
        }
    }
}

void PlayerAliveState::HandleHorizontalMovement(PlayerComponent* playerComp)
{
    glm::vec2 moveDir = playerComp->GetMovingDirection();
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
}

void PlayerAliveState::FixedUpdate(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    PlayerComponent* playerComp = stateMachine.GetComponent();

    HandleGround(playerComp);

    if (playerComp->GetCeilingTrigger()->IsTriggered() && playerComp->IsJumping() && playerComp->GetJumpCorrectionTrigger()->IsTriggered())
    {
        std::cout << "COLLIDER IS OFF\n";

        playerComp->GetCollider()->SetIgnoreStatic(true);
    }
    else if (!playerComp->IsJumping() && !playerComp->GetJumpCorrectionTrigger()->IsTriggered())
    {
        std::cout << "COLLIDER IS ON\n";
        playerComp->GetCollider()->SetIgnoreStatic(false);
    }

    // Clamp player horizontally when needed
    glm::vec2 clampedPos{ playerComp->GetPosition() };
    clampedPos.x = std::min(std::max(clampedPos.x, 34.f), static_cast<float>(Renderer::WIDTH) - playerComp->GetCollider()->GetWidth() - 32.f);
    playerComp->GetOwner()->SetLocalPosition(clampedPos);
}

void PlayerAliveState::HandleGround(PlayerComponent* playerComp)
{
    // Perform raycast downwards to check if the player is grounded
    glm::vec2 rayOrigin = playerComp->GetPosition();
    rayOrigin.x += playerComp->GetCollider()->GetWidth() * 0.5f;
    glm::vec2 rayDirection(0.0f, 1.0f); // Downwards direction
    float rayDistance = 3.f;
    rayOrigin.y += playerComp->GetCollider()->GetHeight() - rayDistance;
    RaycastResult result = Raycast(rayOrigin, rayDirection, rayDistance, CollisionComponent::ColliderType::STATIC);
    if (result.hit)
    {
        if (!playerComp->IsJumping())
        {
            playerComp->SetVerticalVelocity(5);
        }

        playerComp->SetIsGrounded(true);
    }
    else
    {
        // Player is not grounded
        playerComp->SetIsGrounded(false);
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