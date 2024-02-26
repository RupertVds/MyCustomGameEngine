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

    inline const GameObject* GetOwner() const { return m_Owner; }

protected:
    Component() = default;

    GameObject* m_Owner = nullptr;
private:

    virtual void OnInit() {}
    void SetOwner(GameObject* owner) { m_Owner = owner; }
};

