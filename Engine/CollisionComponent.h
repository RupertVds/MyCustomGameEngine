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

    bool IsTrigger() const { return m_IsTrigger; }
    bool IsTriggered() const { return m_IsTriggered; }
    bool IsIgnoringStatic() const { return m_IgnoreStatic; }
    void SetIgnoreStatic(bool isIgnoringStatic) { m_IgnoreStatic = isIgnoringStatic; }
protected:
    explicit CollisionComponent(GameObject* pOwner, ColliderType type, bool isTrigger, ColliderType triggerTargetType)
        : Component(pOwner),
        m_Type{ type }, m_IsTrigger{ isTrigger }, m_TriggerTargetType{ triggerTargetType }
    {}

    ColliderType m_Type{};
    bool m_IsTrigger{};
    bool m_IsTriggered{};
    bool m_IgnoreStatic{};
    ColliderType m_TriggerTargetType{};
};