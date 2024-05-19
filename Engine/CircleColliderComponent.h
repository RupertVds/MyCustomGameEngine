#pragma once
#include "CollisionComponent.h"
#include "ColliderManager.h"
#include <vector>

class BoxColliderComponent;

class CircleColliderComponent final : public CollisionComponent
{
public:
    CircleColliderComponent(GameObject* pOwner, float radius, ColliderType type, bool isTrigger = false);

    virtual ~CircleColliderComponent();

    virtual void FixedUpdate() override;
    virtual void Render() const override;

    bool CheckCircleCollision(CircleColliderComponent* colliderA, CircleColliderComponent* colliderB);

    void ResolveCircleCollision(CircleColliderComponent* colliderA, CircleColliderComponent* colliderB);

    bool CheckCircleBoxCollision(CircleColliderComponent* circle, BoxColliderComponent* box);

    void ResolveCircleBoxCollision(CircleColliderComponent* circle, BoxColliderComponent* box);

    float GetRadius() const { return m_Radius; }

private:
    float m_Radius;
};
