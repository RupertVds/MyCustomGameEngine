#pragma once
#include "CollisionComponent.h"
#include "ColliderManager.h"

class CircleColliderComponent;

class BoxColliderComponent final : public CollisionComponent
{
public:
    BoxColliderComponent(GameObject* pOwner, float width, float height, ColliderType type, bool isTrigger = false);
    ~BoxColliderComponent();

    virtual void FixedUpdate() override;
    virtual void Render() const override;

    float GetWidth() const { return m_Width; }
    float GetHeight() const { return m_Height; }
    glm::vec2 GetPosition() const { return glm::vec2(GetOwner()->GetLocalPosition()); }

private:
    bool CheckCollision(BoxColliderComponent* colliderA, BoxColliderComponent* colliderB);
    void ResolveCollision(BoxColliderComponent* colliderA, BoxColliderComponent* colliderB);
    bool CheckCircleBoxCollision(CircleColliderComponent* circle, BoxColliderComponent* box);
    void ResolveCircleBoxCollision(CircleColliderComponent* circle, BoxColliderComponent* box);

    float m_Width;
    float m_Height;
};
