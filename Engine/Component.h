#pragma once
#include "GameObject.h"

class GameObject;

class Component 
{
public:
    virtual ~Component() = default;
    Component(const Component& other) = delete;
    Component& operator=(const Component& other) = delete;
    Component(Component&& other) noexcept = delete;
    Component& operator=(Component&& other) noexcept = delete;
public:
    virtual void Update() {}
    virtual void FixedUpdate() {}
    virtual void LateUpdate() {}
    virtual void Render() const {}
    virtual void RenderImGui() {}
protected:
    explicit Component(GameObject* pOwner) : m_pOwner{ pOwner } {}
    inline GameObject* const GetOwner() const { if (m_pOwner) return m_pOwner; return nullptr; }
private:
    GameObject* const m_pOwner{};
};