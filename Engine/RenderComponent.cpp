#include "RenderComponent.h"
#include "Renderer.h"

void RenderComponent::Render() const
{
	if (m_Texture != nullptr)
	{
		const auto& pos = GetOwner()->GetPosition();
		Renderer::GetInstance().RenderTexture(*m_Texture.get(), pos.x, pos.y);
	}
}

void RenderComponent::SetTexture(const std::shared_ptr<Texture2D>& texture)
{
	m_Texture = texture;
}

const std::shared_ptr<Texture2D>& RenderComponent::GetTexture() const
{
	return m_Texture;
}
