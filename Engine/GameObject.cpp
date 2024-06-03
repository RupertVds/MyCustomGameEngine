#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <string>
#include <algorithm>
#include "Exceptions.h"
#include "SceneManager.h"
#include <iostream>

GameObject::~GameObject()
{
    std::cout << "DESTRUCTED: " << this << '\n';
}

void GameObject::Update()
{
    if (m_IsMarkedForDeletion) return;

    for (std::unique_ptr<Component>& component : m_Components)
    {
        component->Update();
    }
    
    // Iterate over raw pointers to avoid modifying the container while iterating
    std::vector<GameObject*> childrenCopy;
    childrenCopy.reserve(m_Children.size());
    for (auto& child : m_Children)
    {
        childrenCopy.emplace_back(child.get());
    }

    for (auto& child : childrenCopy)
    {
        child->Update();
    }

    // Remove children marked for deletion
    m_Children.erase(
        std::remove_if(m_Children.begin(), m_Children.end(),
            [](const std::unique_ptr<GameObject>& child) {
                return child->IsMarkedForDeletion();
            }),
        m_Children.end());
}

void GameObject::FixedUpdate()
{
    if (m_IsMarkedForDeletion) return;

    for (std::unique_ptr<Component>& component : m_Components)
    {
        component->FixedUpdate();
    }

    std::vector<GameObject*> childrenCopy;
    childrenCopy.reserve(m_Children.size());
    for (auto& child : m_Children)
    {
        childrenCopy.emplace_back(child.get());
    }

    for (auto& child : childrenCopy)
    {
        child->FixedUpdate();
    }
}

void GameObject::LateUpdate()
{
    if (m_IsMarkedForDeletion) return;

    for (std::unique_ptr<Component>& component : m_Components)
    {
        component->LateUpdate();
    }

    std::vector<GameObject*> childrenCopy;
    childrenCopy.reserve(m_Children.size());
    for (auto& child : m_Children)
    {
        childrenCopy.emplace_back(child.get());
    }

    for (auto& child : childrenCopy)
    {
        child->LateUpdate();
    }
}

void GameObject::Render() const
{
    if (m_IsMarkedForDeletion) return;

    for (const std::unique_ptr<Component>& component : m_Components)
    {
        component->Render();
    }

    for (auto& child : m_Children)
    {
        child->Render();
    }

#if _DEBUG
    // Render position as a dot
    SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Purple color
    SDL_RenderDrawPoint(Renderer::GetInstance().GetSDLRenderer(), static_cast<int>(this->GetLocalPosition().x), static_cast<int>(this->GetLocalPosition().y)); // Draw point at position
#endif
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

void GameObject::AddChild(std::unique_ptr<GameObject>&& pChild) {
    // Check if the child is valid
    if (pChild == nullptr || pChild.get() == this || IsChild(pChild.get())) {
        throw std::invalid_argument("Invalid child");
    }

    // Remove the child from its previous parent
    if (pChild->m_Parent) {
        pChild->m_Parent->RemoveChild(pChild.get());
    }

    // Set this object as the parent of the child
    pChild->m_Parent = this;

    // Add the child to the children list
    m_Children.emplace_back(std::move(pChild));
}

void GameObject::RemoveChild(GameObject* child) {
    auto it = std::find_if(m_Children.begin(), m_Children.end(),
        [child](const std::unique_ptr<GameObject>& c) {
            return c.get() == child;
        });

    if (it != m_Children.end()) {
        (*it)->m_Parent = nullptr; // Remove itself as a parent of the child
        m_Children.erase(it); // Remove the child from the children list
    }
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

GameObject* GameObject::GetChildAtIndex(int index) const
{
    return m_Children[index].get();
}

void GameObject::AddObserver(Observer* observer)
{
    m_Observers.emplace_back(observer);
}

void GameObject::RemoveObserver(Observer* observer)
{
    m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
}

void GameObject::NotifyObservers(Event event)
{
    //if (m_Observers.empty()) throw std::runtime_error("No observers!");
    if (m_Observers.empty()) return;
    for (auto& observer : m_Observers)
    {
        observer->Notify(event, this);
    }
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

void GameObject::SetLocalPosition(const glm::vec2& pos)
{
    m_LocalTransform.SetPosition(pos.x, pos.y);
    SetPositionDirty();
}

 const glm::vec2& GameObject::GetLocalPosition() const
{
    return m_LocalTransform.GetPosition();
}

 const glm::vec2& GameObject::GetWorldPosition()
 {
     if (m_PositionIsDirty)
     {
         UpdateWorldPosition();
     }
     return m_WorldTransform.GetPosition();
 }

 const Transform& GameObject::GetLocalTransform() const
 {
     return m_LocalTransform;
 }

 void GameObject::SetScale(const glm::vec3& scale)
 {
     m_LocalTransform.SetScale(scale.x, scale.y, scale.z);
     m_WorldTransform.SetScale(scale.x, scale.y, scale.z);
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
}


