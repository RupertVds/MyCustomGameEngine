#pragma once
#include "Component.h"

class CollisionComponent : public Component 
{
public:
    enum class ColliderType { STATIC, DYNAMIC };
public:
    virtual ~CollisionComponent() = default;
    virtual void FixedUpdate() override {}
    virtual void OnCollision(CollisionComponent* other) = 0;
    ColliderType GetType() const { return m_Type; }

protected:
    explicit CollisionComponent(GameObject* pOwner, ColliderType type) : Component(pOwner), m_Type{ type} {}
    ColliderType m_Type{};
};