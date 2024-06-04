#include "PlayerStates.h"
#include "Timer.h"
#include <Utils.h>
#include <Renderer.h>
#include "AnimatorComponent.h"
#include "BubbleComponent.h"
#include "ResourceManager.h"

class BoxColliderComponent;

// Definition of methods for PlayerEntryState
void PlayerEntryState::Entry(BehaviorStateMachine<PlayerComponent>& stateMachine) 
{
    std::cout << "PlayerEntryState: Entered" << '\n';
    PlayerComponent* playerComp = stateMachine.GetComponent();
    playerComp->GetAnimator()->Play("JumpDown", true);

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

void PlayerEntryState::FixedUpdate(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    auto playerComp = stateMachine.GetComponent();
    // Apply velocity
    glm::vec2 displacement = playerComp->GetVelocity() * Timer::GetInstance().GetFixedTimeStep(); // Displacement = velocity * time

    // Update position
    glm::vec2 newPosition = glm::vec2(playerComp->GetOwner()->GetWorldPosition()) + displacement;

    // Set the new position
    playerComp->GetOwner()->SetLocalPosition(newPosition);
}

void PlayerEntryState::Exit(BehaviorStateMachine<PlayerComponent>&) 
{
    std::cout << "PlayerEntryState: Exited" << std::endl;
}

// Definition of methods for PlayerAliveState
void PlayerAliveState::Entry(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    std::cout << "PlayerAliveState: Entered" << std::endl;
    m_MoveSpeed = 120.f;
    m_JumpSpeed = 240.f;
    m_JumpTime = 0.35f;
    m_JumpTimeThreshold = m_JumpTime * 0.9f;
    m_FallingSpeed = 140.f;
    stateMachine.GetComponent()->GetAnimator()->Play("Idle");
}

void PlayerAliveState::Update(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    PlayerComponent* playerComp = stateMachine.GetComponent();
    HandleHorizontalMovement(playerComp);
    HandleJumping(playerComp);

    if (!playerComp->IsJumping() && !playerComp->IsGrounded())
    {
        playerComp->SetVerticalVelocity(m_FallingSpeed);
    }

    HandleAnimation(playerComp);

    if (playerComp->IsAttacking() && !playerComp->CanAttack())
    {
        playerComp->GetAnimator()->Play("Attack", false);

        auto bubbleObject = std::make_unique<GameObject>("bubble");

        std::shared_ptr<Texture2D> bubbleCompleteTexture{};
        std::shared_ptr<Texture2D> bubbleFormingTexture{};
        if (playerComp->GetOwner()->GetName() == "player_1")
        {
            bubbleCompleteTexture = ResourceManager::GetInstance().LoadTexture("player_1_bubble.png");
            bubbleFormingTexture = ResourceManager::GetInstance().LoadTexture("player_1_bubble_forming.png");
        }
        else if (playerComp->GetOwner()->GetName() == "player_2")
        {
            bubbleCompleteTexture = ResourceManager::GetInstance().LoadTexture("player_2_bubble.png");
            bubbleFormingTexture = ResourceManager::GetInstance().LoadTexture("player_2_bubble_forming.png");
        }


        bubbleObject->AddComponent<RenderComponent>();
        bubbleObject->SetScale({2.f, 2.f, 2.f});
        auto bubbleAnimator = bubbleObject->AddComponent<AnimatorComponent>(16, 16, 6);
        bubbleObject->AddComponent<CircleColliderComponent>(16.f, CollisionComponent::ColliderType::DYNAMIC, true, CollisionComponent::ColliderType::DYNAMIC);
        
        bubbleAnimator->AddSpriteSheet("forming", bubbleFormingTexture, 12);
        bubbleAnimator->AddSpriteSheet("complete", bubbleCompleteTexture, 1);

        if (playerComp->GetAnimator()->GetRenderComponent()->IsFlipped())
        {
            bubbleObject->AddComponent<BubbleComponent>(glm::vec2{ 1, 0 }, playerComp);
            bubbleObject->SetLocalPosition({ playerComp->GetPosition().x + playerComp->GetCollider()->GetWidth() * 1.25f, playerComp->GetPosition().y});
        }
        else
        {
            bubbleObject->AddComponent<BubbleComponent>(glm::vec2{ -1, 0 }, playerComp);
            bubbleObject->SetLocalPosition({ playerComp->GetPosition().x - playerComp->GetCollider()->GetWidth() * 1.25f, playerComp->GetPosition().y });
        }

        auto scene = SceneManager::GetInstance().GetSceneByName("level");
        scene->Add(std::move(bubbleObject));
        playerComp->SetIsAttacking(false);
    }
    else if (!playerComp->CanAttack())
    {
        if (playerComp->GetAnimator()->ReachedEndFrame())
        {
            playerComp->SetCanAttack(true);
        }
    }
}

void PlayerAliveState::HandleAnimation(PlayerComponent* playerComp)
{
    if (!playerComp->IsAttacking() && playerComp->CanAttack())
    {
        if (playerComp->IsJumping())
        {
            playerComp->GetAnimator()->Play("JumpUp");
        }

        // falling down animation
        if (playerComp->GetVelocity().y > 0 && !playerComp->IsGrounded())
        {
            m_FallingTimer += Timer::GetInstance().GetDeltaTime();

            if (m_FallingTimer >= m_FallingTime)
            {
                playerComp->GetAnimator()->Play("JumpDown");
            }
        }
        else
        {
            m_FallingTimer = 0;
        }

        if (!playerComp->IsJumping() && playerComp->IsGrounded())
        {
            glm::vec2 moveDir = playerComp->GetMovingDirection();
            if (moveDir.x != 0)
            {
                playerComp->GetAnimator()->Play("Run");
            }
            else
            {
                playerComp->GetAnimator()->Play("Idle");
            }
        }
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
        playerComp->GetAnimator()->GetRenderComponent()->SetIsFlipped(true);
    }
    else if (moveDir.x < 0)
    {
        playerComp->SetHorizontalVelocity(-m_MoveSpeed);
        playerComp->GetAnimator()->GetRenderComponent()->SetIsFlipped(false);

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

    if ((playerComp->GetCeilingTrigger()->IsTriggered() && playerComp->IsJumping() && playerComp->GetJumpCorrectionTrigger()->IsTriggered()) || playerComp->GetPosition().y < 0)
    {
        //std::cout << "COLLIDER IS OFF\n";

        playerComp->GetCollider()->SetIgnoreStatic(true);
    }
    else if (!playerComp->IsJumping() && !playerComp->GetJumpCorrectionTrigger()->IsTriggered() && !(playerComp->GetPosition().y < 0))
    {
        //std::cout << "COLLIDER IS ON\n";
        playerComp->GetCollider()->SetIgnoreStatic(false);
    }


    // Apply velocity
    glm::vec2 displacement = playerComp->GetVelocity() * Timer::GetInstance().GetFixedTimeStep(); // Displacement = velocity * time

    // Update position
    glm::vec2 newPosition = glm::vec2(playerComp->GetOwner()->GetWorldPosition()) + displacement;

    // Set the new position
    playerComp->GetOwner()->SetLocalPosition(newPosition);

    // Clamp player horizontally when needed
    glm::vec2 clampedPos{ playerComp->GetPosition() };
    clampedPos.x = std::min(std::max(clampedPos.x, 34.f), static_cast<float>(Renderer::WIDTH) - playerComp->GetCollider()->GetWidth() - 32.f);
    playerComp->GetOwner()->SetLocalPosition(clampedPos);
}

void PlayerAliveState::HandleGround(PlayerComponent* playerComp)
{
    // Get the player's position and collider dimensions
    glm::vec2 position = playerComp->GetPosition() + playerComp->GetCollider()->GetOffset();
    float colliderWidth = playerComp->GetCollider()->GetWidth();
    float colliderHeight = playerComp->GetCollider()->GetHeight();

    // Define the raycast parameters
    glm::vec2 rayDirection(0.0f, 1.0f); // Downwards direction
    float rayDistance = 4.0f;

    // Calculate ray origins for left and right raycasts
    glm::vec2 leftRayOrigin = position;
    leftRayOrigin.x += 1.f;
    leftRayOrigin.y += colliderHeight - rayDistance;

    glm::vec2 rightRayOrigin = position;
    rightRayOrigin.x += colliderWidth - 1.f;
    rightRayOrigin.y += colliderHeight - rayDistance;

    // Perform raycasts
    RaycastResult leftRayResult = Raycast(leftRayOrigin, rayDirection, rayDistance, CollisionComponent::ColliderType::STATIC);
    RaycastResult rightRayResult = Raycast(rightRayOrigin, rayDirection, rayDistance, CollisionComponent::ColliderType::STATIC);

    // Check if either ray hit the ground
    if (leftRayResult.hit || rightRayResult.hit)
    {
        if (!playerComp->IsJumping() && (playerComp->GetPosition().y > 0))
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
void PlayerDeadState::Entry(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    std::cout << "PlayerDeadState: Entered" << std::endl;
    stateMachine.GetComponent()->GetAnimator()->Play("Death", false);
}

void PlayerDeadState::Update(BehaviorStateMachine<PlayerComponent>& stateMachine)
{
    std::cout << "PlayerDeadState: Updated" << std::endl;
    if (stateMachine.GetComponent()->GetAnimator()->ReachedEndFrame())
    {
        stateMachine.SetState(new PlayerEntryState());
    }
    
    //stateMachine.SetState(new PlayerEntryState());
}

void PlayerDeadState::Exit(BehaviorStateMachine<PlayerComponent>&) 
{
    std::cout << "PlayerDeadState: Exited" << std::endl;
}