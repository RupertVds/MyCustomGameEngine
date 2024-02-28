#pragma once
#include "Component.h"
#include "Texture2D.h"
#include "Renderer.h"

class SpriteComponent final : public Component
{
public:
    //=======================================
    // RULE OF ZERO/FIVE/SIX
    //=======================================
    SpriteComponent(GameObject* pOwner, std::shared_ptr<Texture2D> texture) : Component(pOwner), m_Texture(texture) {}
    virtual ~SpriteComponent() = default;
    SpriteComponent(const SpriteComponent& other) = delete;
    SpriteComponent(SpriteComponent&& other) = delete;
    SpriteComponent& operator=(const SpriteComponent& other) = delete;
    SpriteComponent& operator=(SpriteComponent&& other) = delete;
public:
    virtual void Render() const override 
    {
        Renderer::GetInstance().RenderTexture(*m_Texture.get(), GetOwner()->GetPosition().x, GetOwner()->GetPosition().y);
    }

    std::shared_ptr<Texture2D> GetTexture() const {
        return m_Texture;
    }

private:
    std::shared_ptr<Texture2D> m_Texture;
};