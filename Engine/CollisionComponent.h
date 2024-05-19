#pragma once
#include "Component.h"

class CollisionComponent : public Component
{
public:
    enum class ColliderType { STATIC, DYNAMIC };
public:
    virtual ~CollisionComponent() = default;
    virtual void FixedUpdate() override {}
    ColliderType GetType() const { return m_Type; }
    CollisionComponent* IsTriggered(CollisionComponent* other) const;
    bool IsTrigger() const { return m_IsTrigger; }
protected:
    explicit CollisionComponent(GameObject* pOwner, ColliderType type, bool isTrigger) : Component(pOwner), m_Type{ type }, m_IsTrigger{ isTrigger} {}
    ColliderType m_Type{};
    bool m_IsTrigger{};
};