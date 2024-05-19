#include "BoxColliderComponent.h"
#include "Renderer.h"
#include "CircleColliderComponent.h"

BoxColliderComponent::BoxColliderComponent(GameObject* pOwner, float width, float height, ColliderType type, bool isTrigger)
    : CollisionComponent(pOwner, type, isTrigger), m_Width(width), m_Height(height)
{
    ColliderManager::GetInstance().RegisterBoxCollider(this);
}

BoxColliderComponent::~BoxColliderComponent()
{
    ColliderManager::GetInstance().UnregisterBoxCollider(this);
}

void BoxColliderComponent::Render() const
{
#if _DEBUG
    SDL_Renderer* renderer = Renderer::GetInstance().GetSDLRenderer();
    if (!renderer) {
        // Handle renderer not available
        return;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100); // White color
    SDL_Rect rect;
    glm::vec2 position = GetOwner()->GetWorldPosition();
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    rect.w = static_cast<int>(m_Width);
    rect.h = static_cast<int>(m_Height);
    SDL_RenderDrawRect(renderer, &rect);
#endif
}

void BoxColliderComponent::FixedUpdate()
{
    if (m_Type == ColliderType::STATIC && !m_IsTrigger)
        return; // No need to handle collision for static colliders

    // Iterate over all box colliders to check for collisions
    for (auto& collider : ColliderManager::GetInstance().GetBoxColliders()) {
        if (collider == this)
            continue; // Skip self

        if (CheckCollision(this, collider))
        {
            if (!m_IsTrigger && !collider->IsTrigger())
            {
                ResolveCollision(this, collider);
            }
            else
            {
                IsTriggered(collider);
            }
        }
    }

    // Iterate over all circle colliders to check for collisions
    for (auto& collider : ColliderManager::GetInstance().GetCircleColliders()) {
        if (CheckCircleBoxCollision(collider, this))
        {
            if (!m_IsTrigger && !collider->IsTrigger())
            {
                ResolveCircleBoxCollision(collider, this);
            }
            else
            {
                IsTriggered(collider);
            }
        }
    }
}

bool BoxColliderComponent::CheckCollision(BoxColliderComponent* colliderA, BoxColliderComponent* colliderB)
{
    // AABB collision detection
    glm::vec2 posA = colliderA->GetOwner()->GetLocalPosition();
    glm::vec2 posB = colliderB->GetOwner()->GetLocalPosition();

    return (posA.x < posB.x + colliderB->m_Width &&
        posA.x + colliderA->m_Width > posB.x &&
        posA.y < posB.y + colliderB->m_Height &&
        posA.y + colliderA->m_Height > posB.y);
}

void BoxColliderComponent::ResolveCollision(BoxColliderComponent* colliderA, BoxColliderComponent* colliderB)
{
    // Get positions and dimensions of the colliders
    glm::vec2 posA = colliderA->GetOwner()->GetLocalPosition();
    glm::vec2 posB = colliderB->GetOwner()->GetLocalPosition();
    float widthA = colliderA->m_Width;
    float heightA = colliderA->m_Height;
    float widthB = colliderB->m_Width;
    float heightB = colliderB->m_Height;

    // Calculate overlap along the X and Y axes
    float overlapX = std::min(posA.x + widthA, posB.x + widthB) - std::max(posA.x, posB.x);
    float overlapY = std::min(posA.y + heightA, posB.y + heightB) - std::max(posA.y, posB.y);

    // Calculate correction vector
    glm::vec2 correction(0.0f);
    if (overlapX < overlapY)
    {
        if (posA.x < posB.x)
            correction.x = overlapX;
        else
            correction.x = -overlapX;
    }
    else
    {
        if (posA.y < posB.y)
            correction.y = overlapY;
        else
            correction.y = -overlapY;
    }

    // Apply correction with a small threshold to avoid jitter
    float threshold = 0.5f;
    if (std::abs(correction.x) < threshold) correction.x = 0.0f;
    if (std::abs(correction.y) < threshold) correction.y = 0.0f;

    // Apply correction to dynamic collider
    if (colliderA->GetType() == ColliderType::DYNAMIC) {
        posA -= correction;
        colliderA->GetOwner()->SetLocalPosition(posA);
    }
    else if (colliderB->GetType() == ColliderType::DYNAMIC) {
        posB += correction;
        colliderB->GetOwner()->SetLocalPosition(posB);
    }
}

bool BoxColliderComponent::CheckCircleBoxCollision(CircleColliderComponent* circle, BoxColliderComponent* box)
{
    glm::vec2 circlePos = glm::vec2(circle->GetOwner()->GetWorldPosition()) + glm::vec2(circle->GetRadius(), circle->GetRadius());
    glm::vec2 boxPos = box->GetOwner()->GetWorldPosition();
    float radius = circle->GetRadius();
    float boxWidth = box->GetWidth();
    float boxHeight = box->GetHeight();

    // Find the closest point on the box to the circle
    glm::vec2 closestPoint;
    closestPoint.x = std::max(boxPos.x, std::min(circlePos.x, boxPos.x + boxWidth));
    closestPoint.y = std::max(boxPos.y, std::min(circlePos.y, boxPos.y + boxHeight));

    // Calculate the distance between the circle's center and this closest point
    float distance = glm::distance(circlePos, closestPoint);

    // If the distance is less than the circle's radius, an intersection occurs
    return distance < radius;
}

void BoxColliderComponent::ResolveCircleBoxCollision(CircleColliderComponent* circle, BoxColliderComponent* box)
{
    glm::vec2 circlePos = glm::vec2(circle->GetOwner()->GetWorldPosition()) + glm::vec2(circle->GetRadius(), circle->GetRadius());
    glm::vec2 boxPos = box->GetOwner()->GetWorldPosition();
    float radius = circle->GetRadius();
    float boxWidth = box->GetWidth();
    float boxHeight = box->GetHeight();

    // Find the closest point on the box to the circle
    glm::vec2 closestPoint;
    closestPoint.x = std::max(boxPos.x, std::min(circlePos.x, boxPos.x + boxWidth));
    closestPoint.y = std::max(boxPos.y, std::min(circlePos.y, boxPos.y + boxHeight));

    // Calculate the distance between the circle's center and this closest point
    glm::vec2 difference = circlePos - closestPoint;
    float distance = glm::length(difference);

    if (distance == 0.0f) {
        distance = 0.01f; // Prevent division by zero
    }
    glm::vec2 correction = difference * ((radius - distance) / distance);

    if (circle->GetType() == ColliderType::DYNAMIC) {
        circlePos += correction;
        circle->GetOwner()->SetLocalPosition(circlePos);
    }

    if (box->GetType() == ColliderType::DYNAMIC) {
        boxPos -= correction;
        box->GetOwner()->SetLocalPosition(boxPos);
    }
}
