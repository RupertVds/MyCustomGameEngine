#include "CircleColliderComponent.h"
#include "Renderer.h"
#include "BoxColliderComponent.h"
#include <cmath>
#include <algorithm>

CircleColliderComponent::CircleColliderComponent(GameObject* pOwner, float radius, ColliderType type, bool isTrigger, ColliderType triggerTargetType)
    : CollisionComponent(pOwner, type, isTrigger, triggerTargetType), m_Radius(radius)
{
    ColliderManager::GetInstance().RegisterCircleCollider(this);
}

CircleColliderComponent::~CircleColliderComponent()
{
    ColliderManager::GetInstance().UnregisterCircleCollider(this);
}

void CircleColliderComponent::FixedUpdate()
{
    if (m_Type == ColliderType::STATIC && !m_IsTrigger)
        return; // No need to handle collision for static colliders

    bool isTriggered{};

    // Check for collisions with other circle colliders
    for (auto& collider : ColliderManager::GetInstance().GetCircleColliders()) 
    {
        if (collider == this)
            continue; // Skip self and static colliders

        if (m_IgnoreStatic && collider->GetType() == ColliderType::STATIC) continue;

        if (CheckCircleCollision(this, collider))
        {
            if (!m_IsTrigger && !collider->IsTrigger())
            {
                ResolveCircleCollision(this, collider);
            }
            else
            {
                if (collider->GetType() == m_TriggerTargetType)
                {
                    isTriggered = true;
                }
            }
        }
    }

    // Check for collisions with box colliders
    for (auto& collider : ColliderManager::GetInstance().GetBoxColliders()) 
    {
        if (m_IgnoreStatic && collider->GetType() == ColliderType::STATIC) continue;

        if (CheckCircleBoxCollision(this, collider)) 
        {
            if (!m_IsTrigger && !collider->IsTrigger())
            {
                ResolveCircleBoxCollision(this, collider);
            }
            else
            {
                if (collider->GetType() == m_TriggerTargetType)
                {
                    isTriggered = true;
                }
            }
        }
    }

    m_IsTriggered = isTriggered;
}

void CircleColliderComponent::Render() const
{
#if _DEBUG
    SDL_Renderer* renderer = Renderer::GetInstance().GetSDLRenderer();
    if (!renderer) {
        // Handle renderer not available
        return;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100); // White color
    glm::vec2 position = GetOwner()->GetLocalPosition();
    int centerX = static_cast<int>(position.x + m_Radius);
    int centerY = static_cast<int>(position.y + m_Radius);
    int radius = static_cast<int>(m_Radius);

    // Midpoint Circle Algorithm to draw only the outline
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y) {
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }

        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
#endif
}

bool CircleColliderComponent::CheckCircleCollision(CircleColliderComponent* colliderA, CircleColliderComponent* colliderB)
{
    glm::vec2 posA = colliderA->GetOwner()->GetWorldPosition() + glm::vec2(m_Radius, m_Radius);
    glm::vec2 posB = colliderB->GetOwner()->GetWorldPosition() + glm::vec2(m_Radius, m_Radius);
    float radiusA = colliderA->GetRadius();
    float radiusB = colliderB->GetRadius();

    float distance = glm::distance(posA, posB);
    return distance < (radiusA + radiusB);
}

void CircleColliderComponent::ResolveCircleCollision(CircleColliderComponent* colliderA, CircleColliderComponent* colliderB)
{
    glm::vec2 posA = glm::vec2(colliderA->GetOwner()->GetWorldPosition()) + glm::vec2(m_Radius, m_Radius);
    glm::vec2 posB = glm::vec2(colliderB->GetOwner()->GetWorldPosition()) + glm::vec2(m_Radius, m_Radius);
    float radiusA = colliderA->GetRadius();
    float radiusB = colliderB->GetRadius();

    glm::vec2 difference = posA - posB;
    float distance = glm::length(difference);
    if (distance == 0.0f) {
        distance = 0.01f; // Prevent division by zero
    }
    glm::vec2 correction = difference * ((radiusA + radiusB - distance) / distance) * 0.5f;

    if (colliderA->GetType() == ColliderType::DYNAMIC) {
        posA += correction;
        colliderA->GetOwner()->SetLocalPosition(posA - glm::vec2(m_Radius, m_Radius));
    }

    if (colliderB->GetType() == ColliderType::DYNAMIC) {
        posB -= correction;
        colliderB->GetOwner()->SetLocalPosition(posB - glm::vec2(m_Radius, m_Radius));
    }
}

bool CircleColliderComponent::CheckCircleBoxCollision(CircleColliderComponent* circle, BoxColliderComponent* box)
{
    glm::vec2 circlePos = glm::vec2(circle->GetOwner()->GetWorldPosition()) + glm::vec2(circle->GetRadius(), circle->GetRadius());
    glm::vec2 boxPos = box->GetOwner()->GetWorldPosition();
    float radius = circle->GetRadius();

    // Find the closest point to the circle within the box
    glm::vec2 closestPoint;
    closestPoint.x = std::max(boxPos.x, std::min(circlePos.x, boxPos.x + box->GetWidth()));
    closestPoint.y = std::max(boxPos.y, std::min(circlePos.y, boxPos.y + box->GetHeight()));

    float distance = glm::distance(circlePos, closestPoint);

    return distance < radius;
}

void CircleColliderComponent::ResolveCircleBoxCollision(CircleColliderComponent* circle, BoxColliderComponent* box)
{
    glm::vec2 circlePos = glm::vec2(circle->GetOwner()->GetWorldPosition()) + glm::vec2(circle->GetRadius(), circle->GetRadius());
    glm::vec2 boxPos = box->GetOwner()->GetWorldPosition();
    float radius = circle->GetRadius();

    // Find the closest point to the circle within the box
    glm::vec2 closestPoint;
    closestPoint.x = std::max(boxPos.x, std::min(circlePos.x, boxPos.x + box->GetWidth()));
    closestPoint.y = std::max(boxPos.y, std::min(circlePos.y, boxPos.y + box->GetHeight()));

    glm::vec2 difference = circlePos - closestPoint;
    float distance = glm::length(difference);

    if (distance == 0.0f) {
        distance = 0.01f; // Prevent division by zero
    }
    glm::vec2 correction = difference * ((radius - distance) / distance);

    if (circle->GetType() == ColliderType::DYNAMIC) {
        circlePos += correction;
        circle->GetOwner()->SetLocalPosition(circlePos - glm::vec2(radius, radius));
    }

    if (box->GetType() == ColliderType::DYNAMIC) {
        boxPos -= correction;
        box->GetOwner()->SetLocalPosition(boxPos);
    }
}
