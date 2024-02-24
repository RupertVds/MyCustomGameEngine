#pragma once
#include "GameObject.h"

class GameObject;

class Component 
{
    friend class GameObject;
public:
    virtual void OnInit() {}
    virtual void Update() {}
    virtual void FixedUpdate() {}
    virtual void LateUpdate() {}
    virtual void Render() const {}

    inline const GameObject* GetOwner() const { return m_Owner; }

    virtual ~Component() = default;
    
protected:
    GameObject* m_Owner = nullptr;
private:
    void SetOwner(GameObject* owner) { m_Owner = owner; OnInit(); }
};

