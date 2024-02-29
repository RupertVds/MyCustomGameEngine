#pragma once
#include "GameObject.h"

class GameObject;

class Component 
{
    //friend class GameObject;
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
protected:
    virtual void OnInit() {}
    Component(GameObject* pOwner) : m_pOwner{ pOwner } {}
    inline GameObject* const GetOwner() const { return m_pOwner; }
private:
    GameObject* const m_pOwner = nullptr;
};