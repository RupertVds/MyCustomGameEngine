#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <string>
#include <algorithm>
#include "Exceptions.h"
#include "SceneManager.h"

void GameObject::Update()
{
    for (std::unique_ptr<Component>& component : m_Components)
    {
        component->Update();
    }
    
    for (auto& child : m_Children)
    {
        child->Update();
    }
}

void GameObject::FixedUpdate()
{
    for (std::unique_ptr<Component>& component : m_Components)
    {
        component->FixedUpdate();
    }

    for (auto& child : m_Children)
    {
        child->FixedUpdate();
    }
}

void GameObject::LateUpdate()
{
    for (std::unique_ptr<Component>& component : m_Components)
    {
        component->LateUpdate();
    }

    for (auto& child : m_Children)
    {
        child->LateUpdate();
    }
}

void GameObject::Render() const
{
    for (const std::unique_ptr<Component>& component : m_Components)
    {
        component->Render();
    }

    for (auto& child : m_Children)
    {
        child->Render();
    }
}

void GameObject::RenderImGui()
{
    for (const std::unique_ptr<Component>& component : m_Components)
    {
        component->RenderImGui();
    }

    for (auto& child : m_Children)
    {
        child->RenderImGui();
    }
}

GameObject* GameObject::GetParent() const
{
    return m_Parent;
}

void GameObject::SetParent(std::shared_ptr<GameObject> parent, bool keepWorldPosition)
{
    if (parent == nullptr)
    {
        SetParent(SceneManager::GetInstance().GetRootObject());
    }
    else
    {
        if (keepWorldPosition)
        {
            SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
            SetPositionDirty();
        }
    }

    if (parent.get() == m_Parent ||
        parent.get() == this ||
        IsChild(parent.get()))
    {
        throw SetParentIsInvalidException();
    }

    if (m_Parent) m_Parent->RemoveChild(shared_from_this());

    m_Parent = parent.get();

    if (m_Parent) m_Parent->AddChild(shared_from_this());
}

bool GameObject::IsChild(GameObject* parent)
{
    auto it = std::find_if(m_Children.begin(), m_Children.end(),
        [parent](auto& child) {
            return child.get() == parent;
        });

    return it != m_Children.end();
}

size_t GameObject::GetChildCount() const
{
    return m_Children.size();
}

const std::vector<std::shared_ptr<GameObject>>& GameObject::GetChildren() const
{
    return m_Children;
}

const std::shared_ptr<GameObject>& GameObject::GetChildAtIndex(int index) const
{
    return m_Children[index];
}

void GameObject::AddObserver(std::shared_ptr<Observer> observer)
{
    m_Observers.emplace_back(observer);
    
}

void GameObject::RemoveObserver(std::shared_ptr<Observer> observer)
{
    m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
}

void GameObject::NotifyObservers(Event event)
{
    if (m_Observers.empty()) throw std::runtime_error("No observers!");
    for (auto& observer : m_Observers)
    {
        observer->Notify(event, this);
    }
}

void GameObject::AddChild(std::shared_ptr<GameObject> child)
{
    m_Children.emplace_back(child);
}

void GameObject::RemoveChild(std::shared_ptr<GameObject> child)
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
    for (auto& child : m_Children)
    {
        child->SetPositionDirty();
    }
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
    if (m_IsMarkedForDeletion) return;
    m_IsMarkedForDeletion = true;

    for (auto& child : m_Children)
    {
        child->DeleteSelf();
    }
    //m_Children.clear();

    if (m_Parent)
    {
        m_Parent->RemoveChild(shared_from_this());
    }
}


