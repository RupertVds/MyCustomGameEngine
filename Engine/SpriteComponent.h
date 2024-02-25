#pragma once
#include "Component.h"
#include "Texture2D.h"
#include "Renderer.h"

class SpriteComponent final : public Component
{
public:
    SpriteComponent(std::shared_ptr<Texture2D> texture) : m_Texture(texture) {}

    virtual void Render() const override 
    {
        Renderer::GetInstance().RenderTexture(*m_Texture.get(), m_Owner->GetPosition().x, m_Owner->GetPosition().y);
    }

    std::shared_ptr<Texture2D> GetTexture() const {
        return m_Texture;
    }

private:
    std::shared_ptr<Texture2D> m_Texture;
};