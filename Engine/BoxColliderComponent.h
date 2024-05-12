#pragma once
#include "CollisionComponent.h"
#include "ColliderManager.h"
#include "Renderer.h"

class BoxColliderComponent : public CollisionComponent
{
public:
    BoxColliderComponent(GameObject* pOwner, float width, float height, ColliderType type)
        : CollisionComponent(pOwner, type), m_Width(width), m_Height(height)
    {
        ColliderManager::GetInstance().RegisterBoxCollider(this);
    }

    ~BoxColliderComponent() {
        ColliderManager::GetInstance().UnregisterBoxCollider(this);
    }

    virtual void Render() const override {
        SDL_Renderer* renderer = Renderer::GetInstance().GetSDLRenderer();
        if (!renderer) {
            // Handle renderer not available
            return;
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
        SDL_Rect rect;
        glm::vec2 position = GetOwner()->GetLocalPosition();
        rect.x = static_cast<int>(position.x);
        rect.y = static_cast<int>(position.y);
        rect.w = static_cast<int>(m_Width);
        rect.h = static_cast<int>(m_Height);
        SDL_RenderDrawRect(renderer, &rect);
    }

    virtual void FixedUpdate() override {
        if (m_Type == ColliderType::STATIC)
            return; // No need to handle collision for static colliders

        // Iterate over all box colliders to check for collisions
        for (auto& collider : ColliderManager::GetInstance().GetBoxColliders()) {
            if (collider == this || collider->GetType() == ColliderType::STATIC)
                continue; // Skip self and static colliders

            if (CheckCollision(this, collider)) {
                ResolveCollision(this, collider);
            }
        }
    }

    virtual void OnCollision(CollisionComponent*) override {
        // Handle collision response if needed
    }

private:
    float m_Width;
    float m_Height;
    ColliderType m_Type;

    bool CheckCollision(BoxColliderComponent* colliderA, BoxColliderComponent* colliderB) {
        // AABB collision detection
        glm::vec2 posA = colliderA->GetOwner()->GetLocalPosition();
        glm::vec2 posB = colliderB->GetOwner()->GetLocalPosition();

        return (posA.x < posB.x + colliderB->m_Width &&
            posA.x + colliderA->m_Width > posB.x &&
            posA.y < posB.y + colliderB->m_Height &&
            posA.y + colliderA->m_Height > posB.y);
    }

    void ResolveCollision(BoxColliderComponent* colliderA, BoxColliderComponent* colliderB) {
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
        if (overlapX < overlapY) {
            if (posA.x < posB.x)
                correction.x = overlapX;
            else
                correction.x = -overlapX;
        }
        else {
            if (posA.y < posB.y)
                correction.y = overlapY;
            else
                correction.y = -overlapY;
        }

        // Apply correction to dynamic collider
        if (colliderA->GetType() == ColliderType::DYNAMIC) {
            posA -= correction;
            colliderA->GetOwner()->SetLocalPosition({ posA, 0 });
        }
        else if (colliderB->GetType() == ColliderType::DYNAMIC) {
            posB += correction;
            colliderB->GetOwner()->SetLocalPosition({ posB, 0 });
        }
    }

};
