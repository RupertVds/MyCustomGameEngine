#include "RenderComponent.h"
#include "Renderer.h"

RenderComponent::RenderComponent(GameObject* pOwner, std::shared_ptr<Texture2D> texture)
    :
    Component(pOwner),
    m_Texture{texture}
{
    if (m_Texture != nullptr)
    {
        const auto& size = texture->GetSize();
        m_SrcRect = { 0, 0, size.x, size.y}; // Source rect starts at top-left corner and covers entire texture
        m_DestRect = { 0, 0, 
            size.x * static_cast<int>(pOwner->GetLocalTransform().GetScale().x), 
            size.y * static_cast<int>(pOwner->GetLocalTransform().GetScale().y) }; // Destination rect starts at top-left corner with the same size as texture
    }
}

void RenderComponent::Render() const
{
    if (m_Texture != nullptr)
    {
        const auto& pos = GetOwner()->GetWorldPosition();
        Renderer::GetInstance().RenderTexture(*m_Texture.get(), m_SrcRect, { static_cast<int>(pos.x), static_cast<int>(pos.y),
            m_DestRect.w * static_cast<int>(GetOwner()->GetLocalTransform().GetScale().x),
            m_DestRect.h * static_cast<int>(GetOwner()->GetLocalTransform().GetScale().y) });
    }
}

void RenderComponent::SetTexture(const std::shared_ptr<Texture2D>& texture)
{
    m_Texture = texture;
}

void RenderComponent::SetSrcRect(const SDL_Rect& srcRect)
{
    m_SrcRect = srcRect;
}

void RenderComponent::SetDestRect(const SDL_Rect& destRect)
{
    m_DestRect = destRect;
}

const std::shared_ptr<Texture2D>& RenderComponent::GetTexture() const
{
    return m_Texture;
}

const SDL_Rect& RenderComponent::GetSrcRect() const
{
    return m_SrcRect;
}

const SDL_Rect& RenderComponent::GetDestRect() const
{
    return m_DestRect;
}
