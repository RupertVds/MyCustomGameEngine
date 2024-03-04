#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <string>
#include <algorithm>
#include "Exceptions.h"

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


GameObject* GameObject::GetParent() const
{
    return m_Parent;
}

void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
    if (parent == m_Parent ||
        parent == this ||
        IsChild(parent))
    {
        throw SetParentIsInvalidException();
    }

    if (parent == nullptr)
    {
        SetLocalPosition(GetWorldPosition());
    }
    else
    {
        if (keepWorldPosition)
        {
            SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
            SetPositionDirty();

        }
    }

    if(m_Parent) parent->RemoveChild(this);

    m_Parent = parent;

    if (m_Parent) parent->AddChild(this);

}

bool GameObject::IsChild(GameObject*& parent)
{
    return std::find(m_Children.begin(), m_Children.end(), parent) != m_Children.end();
}

size_t GameObject::GetChildCount() const
{
    return m_Children.size();
}

const std::vector<GameObject*>& GameObject::GetChildren() const
{
    return m_Children;
}

GameObject* GameObject::GetChildAtIndex(int index) const
{
    return m_Children[index];
}

void GameObject::AddChild(GameObject* child)
{
    m_Children.emplace_back(std::move(child));
}

void GameObject::RemoveChild(GameObject* child)
{
    m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child), m_Children.end());
}

void GameObject::UpdateWorldPosition()
{
    if (m_PositionIsDirty)
    {
        if (m_Parent == nullptr)
            m_WorldTransform.SetPosition(GetLocalPosition());
        else
            m_WorldTransform.SetPosition((m_Parent->GetWorldPosition() + GetLocalPosition()));
    }
    m_PositionIsDirty = false;
}

void GameObject::SetPositionDirty()
{
    m_PositionIsDirty = true;
}

void GameObject::SetLocalPosition(const glm::vec3& pos)
{
    m_LocalTransform.SetPosition(pos.x, pos.y, pos.z);
    SetPositionDirty();
}

 const glm::vec3& GameObject::GetLocalPosition() const
{
    return m_LocalTransform.GetPosition();
}

 const glm::vec3& GameObject::GetWorldPosition()
 {
     if (m_PositionIsDirty)
     {
         UpdateWorldPosition();
     }
     return m_WorldTransform.GetPosition();
 }

 bool GameObject::IsMarkedForDeletion() const
{
    return m_IsMarkedForDeletion;
}

void GameObject::DeleteSelf()
{
    m_IsMarkedForDeletion = true;
}


