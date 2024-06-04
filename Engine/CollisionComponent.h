#pragma once
#include "Component.h"

class CollisionComponent : public Component
{
public:
    enum class ColliderType { STATIC, DYNAMIC };
public:
    virtual ~CollisionComponent() = default;
    virtual void FixedUpdate() override {}
    virtual void LateUpdate() override { m_TriggeredObjects.clear(); }
    ColliderType GetType() const { return m_Type; }

    bool IsTrigger() const { return m_IsTrigger; }
    bool IsTriggered() const { return m_IsTriggered; }
    const std::vector<GameObject*>& GetTriggeredObjects() const { return m_TriggeredObjects; }

    bool IsIgnoringStatic() const { return m_IgnoreStatic; }
    void SetIgnoreStatic(bool isIgnoringStatic) { m_IgnoreStatic = isIgnoringStatic; }

    const glm::vec2& GetOffset() const { return m_Offset; }
    void SetOffset(const glm::vec2& offset) { m_Offset = offset; }

    void SetIsDisabled(bool isDisabled) { m_IsDisabled = isDisabled; }
protected:
    explicit CollisionComponent(GameObject* pOwner, ColliderType type, bool isTrigger, ColliderType triggerTargetType)
        : Component(pOwner),
        m_Type{ type }, m_IsTrigger{ isTrigger }, m_TriggerTargetType{ triggerTargetType }
    {}

    bool m_IsDisabled{};
    ColliderType m_Type{};
    bool m_IsTrigger{};
    bool m_IsTriggered{};
    bool m_IgnoreStatic{};
    ColliderType m_TriggerTargetType{};
    std::vector<GameObject*> m_TriggeredObjects{};
    glm::vec2 m_Offset{};
};