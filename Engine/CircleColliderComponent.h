#pragma once
#pragma once
#include "CollisionComponent.h"

class CircleColliderComponent : public CollisionComponent
{
public:
    CircleColliderComponent(GameObject* pOwner, float width, float height, ColliderType type)
        : CollisionComponent(pOwner, type), m_Width(width), m_Height(height) {}

    virtual void FixedUpdate() override {
    }

    virtual void OnCollision(CollisionComponent*) override {
        // Handle collision response if needed
    }

    void AddCollider(CollisionComponent* collider) {
        m_Colliders.push_back(collider);
    }


private:
    float m_Width;
    float m_Height;
    std::vector<CollisionComponent*> m_Colliders; // Store references to other colliders
    
};