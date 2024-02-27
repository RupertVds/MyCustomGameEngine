#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

GameObject::~GameObject() = default;

void GameObject::Update()
{
    for (std::unique_ptr<Component>& component : m_Components)
    {
        component->Update();
    }
}

void GameObject::FixedUpdate()
{
    for (std::unique_ptr<Component>& component : m_Components)
    {
        component->FixedUpdate();
    }
}

void GameObject::LateUpdate()
{
    for (std::unique_ptr<Component>& component : m_Components)
    {
        component->LateUpdate();
    }
}

void GameObject::Render() const
{
    for (const std::unique_ptr<Component>& component : m_Components)
    {
        component->Render();
    }
}

void GameObject::SetPosition(float x, float y)
{
    m_Transform.SetPosition(x, y, 0);
}

 const glm::vec3& GameObject::GetPosition() const
{
    return m_Transform.GetPosition();
}

 const Transform& GameObject::GetTransform() const
{
    return m_Transform;
}

 bool GameObject::IsMarkedForDeletion() const
{
    return m_IsMarkedForDeletion;
}

void GameObject::DeleteSelf()
{
    m_IsMarkedForDeletion = true;
}


