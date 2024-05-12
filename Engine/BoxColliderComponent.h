#pragma once
#include "CollisionComponent.h"
#include "ColliderManager.h"
#include "Renderer.h"

class BoxColliderComponent : public CollisionComponent
{
public:
    BoxColliderComponent(GameObject* pOwner, float width, float height, ColliderType type);

    ~BoxColliderComponent();

    virtual void Render() const override;

    virtual void FixedUpdate() override;

    virtual void OnCollision(CollisionComponent*) override {
        // Handle collision response if needed
    }

    float GetWidth() const { return m_Width; }
    float GetHeight() const { return m_Height; }
    glm::vec2 GetPosition() const { return glm::vec2(GetOwner()->GetLocalPosition()); }

private:
    float m_Width;
    float m_Height;
    ColliderType m_Type;

    bool CheckCollision(BoxColliderComponent* colliderA, BoxColliderComponent* colliderB);

    void ResolveCollision(BoxColliderComponent* colliderA, BoxColliderComponent* colliderB);
};
