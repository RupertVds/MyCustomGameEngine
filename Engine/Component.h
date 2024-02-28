#pragma once
#include "GameObject.h"

class Component 
{
    friend class GameObject;
public:
    //=======================================
    // RULE OF ZERO/FIVE/SIX
    //=======================================
    virtual ~Component() = default;
    Component(const Component& other) = delete;
    Component& operator=(const Component& other) = delete;
    Component(Component&& other) = delete;
    Component& operator=(Component&& other) = delete;
public:
    virtual void Update() {}
    virtual void FixedUpdate() {}
    virtual void LateUpdate() {}
    virtual void Render() const {}
protected:
    Component(GameObject* pOwner) { m_pOwner = pOwner; OnInit(); };
    inline GameObject* GetOwner() const { return m_pOwner; }
private:
    GameObject* m_pOwner = nullptr;
    virtual void OnInit() {}
};