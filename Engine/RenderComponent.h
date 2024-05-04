#pragma once
#include "Component.h"
#include "Texture2D.h"
#include <SDL.h>

class RenderComponent final : public Component
{
public:
    RenderComponent(GameObject* pOwner, std::shared_ptr<Texture2D> texture = nullptr);
public:
    virtual void Render() const override;
    void SetTexture(const std::shared_ptr<Texture2D>& texture);
    void SetSrcRect(const SDL_Rect& srcRect);
    void SetDestRect(const SDL_Rect& destRect);
    const std::shared_ptr<Texture2D>& GetTexture() const;

    const SDL_Rect& GetSrcRect() const;

    const SDL_Rect& GetDestRect() const;

private:
    std::shared_ptr<Texture2D> m_Texture;
    SDL_Rect m_SrcRect{};
    SDL_Rect m_DestRect{};
};