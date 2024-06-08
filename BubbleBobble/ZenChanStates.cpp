#include "ZenChanStates.h"
#include <iostream>
#include "Timer.h"
#include <Utils.h>
#include "AnimatorComponent.h"

class BoxColliderComponent;


void ZenChanEntryState::Entry(BehaviorStateMachine<ZenChanComponent>& stateMachine)
{
    std::cout << "ZenChanEntryState: Entered" << '\n';
    auto zenChanComp = stateMachine.GetComponent();

    // Start the downward movement
    zenChanComp->SetVelocity({ 0, m_MovingDownSpeed });
    //zenChanComp->GetCollider()->SetIgnoreStatic(true);
}

void ZenChanEntryState::Update(BehaviorStateMachine<ZenChanComponent>&)
{
    m_MovingDownElapsedTime += Timer::GetInstance().GetDeltaTime();
}

void ZenChanEntryState::FixedUpdate(BehaviorStateMachine<ZenChanComponent>& stateMachine)
{
    ZenChanComponent* zenChanComp = stateMachine.GetComponent();

    // Apply velocity
    glm::vec2 displacement = zenChanComp->GetVelocity() * Timer::GetInstance().GetFixedTimeStep(); // Displacement = velocity * time
    glm::vec2 newPosition = glm::vec2(zenChanComp->GetOwner()->GetWorldPosition()) + displacement;

    // Set the new position
    zenChanComp->GetOwner()->SetLocalPosition(newPosition);

    // Perform ground check
    HandleGround(zenChanComp);

    // Transition to Wander state if grounded and time has elapsed
    if (zenChanComp->IsGrounded() && m_MovingDownElapsedTime >= m_MovingDownTime)
    {
        stateMachine.SetState(new ZenChanWanderState());
    }
}

void ZenChanEntryState::Exit(BehaviorStateMachine<ZenChanComponent>&)
{
}

void ZenChanEntryState::HandleGround(ZenChanComponent* zenChanComp)
{
    glm::vec2 position = zenChanComp->GetPosition() + zenChanComp->GetCollider()->GetOffset();
    float colliderWidth = zenChanComp->GetCollider()->GetWidth();
    float colliderHeight = zenChanComp->GetCollider()->GetHeight();

    glm::vec2 rayDirection(0.0f, 1.0f); // Downwards direction
    float rayDistance = 4.0f;

    glm::vec2 leftRayOrigin = position;
    leftRayOrigin.x += 1.f;
    leftRayOrigin.y += colliderHeight - rayDistance;

    glm::vec2 rightRayOrigin = position;
    rightRayOrigin.x += colliderWidth - 1.f;
    rightRayOrigin.y += colliderHeight - rayDistance;

    RaycastResult leftRayResult = Raycast(leftRayOrigin, rayDirection, rayDistance, CollisionComponent::ColliderType::STATIC);
    RaycastResult rightRayResult = Raycast(rightRayOrigin, rayDirection, rayDistance, CollisionComponent::ColliderType::STATIC);

    if (leftRayResult.hit || rightRayResult.hit)
    {
        zenChanComp->SetIsGrounded(true);
    }
    else
    {
        zenChanComp->SetIsGrounded(false);
    }
}

void ZenChanWanderState::Entry(BehaviorStateMachine<ZenChanComponent>& stateMachine)
{
    std::cout << "ZenChanWanderState: Entered" << '\n';
    ZenChanComponent* zenChanComp = stateMachine.GetComponent();
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    (rand() % 2) ? zenChanComp->SetHorizontalVelocity(m_MoveSpeed) : zenChanComp->SetHorizontalVelocity(-m_MoveSpeed);
    m_CurrentJumpInterval = m_MinJumpInterval + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (m_MaxJumpInterval - m_MinJumpInterval)));

}

void ZenChanWanderState::Update(BehaviorStateMachine<ZenChanComponent>& stateMachine)
{
    ZenChanComponent* zenChanComp = stateMachine.GetComponent();

    HandleHorizontalMovement(zenChanComp);
    HandleJumping(zenChanComp);

    if (!m_IsJumping && !zenChanComp->IsGrounded())
    {
        zenChanComp->SetVerticalVelocity(m_FallingSpeed);
    }

    if (!m_IsJumping && zenChanComp->IsGrounded())
    {
        m_JumpDelayElapsedTime += Timer::GetInstance().GetDeltaTime();
    }


    // Perform ground check
    HandleGround(zenChanComp);

    // Check for jumping
    if (!m_IsJumping && m_JumpDelayElapsedTime >= m_CurrentJumpInterval && zenChanComp->IsGrounded() && m_IsUnderCeiling)
    {
        std::cout << "Preparing to Jump\n";
        m_IsJumping = true;
        m_IsUnderCeiling = false;
        m_JumpDelayElapsedTime = 0;
        m_CurrentJumpInterval = m_MinJumpInterval + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (m_MaxJumpInterval - m_MinJumpInterval)));
    }
}

void ZenChanWanderState::FixedUpdate(BehaviorStateMachine<ZenChanComponent>& stateMachine)
{
    ZenChanComponent* zenChanComp = stateMachine.GetComponent();

    // Apply velocity
    glm::vec2 displacement = zenChanComp->GetVelocity() * Timer::GetInstance().GetFixedTimeStep(); // Displacement = velocity * time

    if (m_IsJumping)
    {
        displacement.x = 0;
    }

    glm::vec2 newPosition = glm::vec2(zenChanComp->GetOwner()->GetWorldPosition()) + displacement;

    // Set the new position
    zenChanComp->GetOwner()->SetLocalPosition(newPosition);
}

void ZenChanWanderState::LateUpdate(BehaviorStateMachine<ZenChanComponent>&)
{

}

void ZenChanWanderState::Exit(BehaviorStateMachine<ZenChanComponent>&)
{
    std::cout << "ZenChanWanderState: Exited" << '\n';
}

void ZenChanWanderState::HandleGround(ZenChanComponent* zenChanComp)
{
    glm::vec2 position = zenChanComp->GetPosition() + zenChanComp->GetCollider()->GetOffset();
    float colliderWidth = zenChanComp->GetCollider()->GetWidth();
    float colliderHeight = zenChanComp->GetCollider()->GetHeight();

    glm::vec2 rayDirection(0.0f, 1.0f); // Downwards direction
    float rayDistance = 3.0f;

    glm::vec2 leftRayOrigin = position;
    leftRayOrigin.x += 1.f;
    leftRayOrigin.y += colliderHeight - rayDistance;

    glm::vec2 rightRayOrigin = position;
    rightRayOrigin.x += colliderWidth - 1.f;
    rightRayOrigin.y += colliderHeight - rayDistance;

    RaycastResult leftRayResult = Raycast(leftRayOrigin, rayDirection, rayDistance, CollisionComponent::ColliderType::STATIC);
    RaycastResult rightRayResult = Raycast(rightRayOrigin, rayDirection, rayDistance, CollisionComponent::ColliderType::STATIC);

    if (leftRayResult.hit || rightRayResult.hit)
    {
        zenChanComp->SetIsGrounded(true);
        if (!m_IsJumping)
        {
            zenChanComp->SetVerticalVelocity(5);
        }
    }
    else
    {
        zenChanComp->SetIsGrounded(false);
    }
}

void ZenChanWanderState::HandleJumping(ZenChanComponent* zenChanComp)
{
    glm::vec2 position = zenChanComp->GetPosition() + zenChanComp->GetCollider()->GetOffset();
    float colliderWidth = zenChanComp->GetCollider()->GetWidth();
    float colliderHeight = zenChanComp->GetCollider()->GetHeight();

    glm::vec2 rayDirection(0.0f, -1.0f); //Upwards direction
    float rayDistance = colliderHeight * 1.1f;

    glm::vec2 middleRayOrigin = position;
    middleRayOrigin.x += colliderWidth * 0.5f;

    RaycastResult middleRayResult = Raycast(middleRayOrigin, rayDirection, rayDistance, CollisionComponent::ColliderType::STATIC);

    if (middleRayResult.hit)
    {
        std::cout << "IS UNDER CEILING\n";
        m_IsUnderCeiling = true;
    }
    else
    {
        m_IsUnderCeiling = false;
    }

    if (m_IsJumping)
    {
        // Handle jumping logic
        zenChanComp->SetVerticalVelocity(-m_JumpSpeed);
        zenChanComp->GetCollider()->SetIgnoreStatic(true);
        // Increment jump timer
        m_JumpElapsedTime += Timer::GetInstance().GetDeltaTime();

        // Check if jump time has elapsed
        if (m_JumpElapsedTime >= m_JumpTime)
        {
            // Reset jump state
            std::cout << "Jump completed\n";
            m_IsJumping = false;
            m_JumpElapsedTime = 0;
            zenChanComp->SetVerticalVelocity(m_FallingSpeed);

            (rand() % 2) ? zenChanComp->SetHorizontalVelocity(m_MoveSpeed) : zenChanComp->SetHorizontalVelocity(-m_MoveSpeed);
            
            zenChanComp->GetCollider()->SetIgnoreStatic(false);
        }
    }
}

void ZenChanWanderState::HandleHorizontalMovement(ZenChanComponent* zenChanComp)
{
    //std::cout << "Velocity Y: " << zenChanComp->GetVelocity().y << '\n';

    if (!m_IsJumping)
    {
        glm::vec2 position = zenChanComp->GetPosition() + zenChanComp->GetCollider()->GetOffset();
        float colliderWidth = zenChanComp->GetCollider()->GetWidth();
        float colliderHeight = zenChanComp->GetCollider()->GetHeight();

        glm::vec2 rayDirection{ 1.0f , 0.0f };
        float rayReach = 2.f;
        float rayDistance = colliderWidth + rayReach;

        glm::vec2 middleRayOrigin = position;
        middleRayOrigin.x -= rayReach * 0.5f;
        middleRayOrigin.y += colliderHeight * 0.5f;

        RaycastResult middleRayResult = Raycast(middleRayOrigin, rayDirection, rayDistance, CollisionComponent::ColliderType::STATIC);

        if (middleRayResult.hit)
        {
            std::cout << "Hit Detected! Reversing Direction.\n";
            zenChanComp->SetHorizontalVelocity(-zenChanComp->GetVelocity().x);
        }

        // Ensure velocity is never zero
        if (zenChanComp->GetVelocity().x == 0)
        {
            std::cout << "Velocity is zero. Setting a new random direction.\n";
            (rand() % 2) ? zenChanComp->SetHorizontalVelocity(m_MoveSpeed) : zenChanComp->SetHorizontalVelocity(-m_MoveSpeed);
        }

        if (zenChanComp->GetVelocity().x > 0)
        {
            zenChanComp->GetAnimator()->GetRenderComponent()->SetIsFlipped(true);
        }
        else
        {
            zenChanComp->GetAnimator()->GetRenderComponent()->SetIsFlipped(false);
        }
    }
}
