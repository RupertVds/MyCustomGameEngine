#pragma once
#include "Component.h"
#include "Texture2D.h"

class RenderComponent final : public Component
{
public:
    RenderComponent(GameObject* pOwner, std::shared_ptr<Texture2D> texture = nullptr) : Component(pOwner), m_Texture(texture) {}
public:
    virtual void Render() const override;
    void SetTexture(const std::shared_ptr<Texture2D>& texture);
    const std::shared_ptr<Texture2D>& GetTexture() const;

private:
    std::shared_ptr<Texture2D> m_Texture;
};