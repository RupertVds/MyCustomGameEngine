#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "Timer.h"
#include <stdexcept>
#include <SDL_ttf.h>

TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font)
	: 
	Component(pOwner),
	m_NeedsUpdate(true), m_Text(text), m_Font(std::move(font))
{
	m_RenderComponent = GetOwner()->GetComponent<RenderComponent>();
	UpdateTexture();
}

void TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		UpdateTexture();
		m_NeedsUpdate = false;
	}
}

void TextComponent::UpdateTexture()
{
    const SDL_Color color = { 255,255,255,255 }; // White text color
    const auto surf = TTF_RenderText_Blended_Wrapped(m_Font->GetFont(), m_Text.c_str(), color, 9999);
    if (surf == nullptr)
    {
        throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
    }

    auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
    if (texture == nullptr)
    {
        SDL_FreeSurface(surf);
        throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
    }

    // Set the texture to the RenderComponent
    m_RenderComponent->SetTexture(std::make_shared<Texture2D>(texture));

    // Update destination rectangle based on text surface size
    SDL_Rect destRect = { 0, 0, surf->w, surf->h }; // Set destination rectangle to match text surface size

    // Optionally, update source rectangle (for later use)
    SDL_Rect srcRect = { 0, 0, surf->w, surf->h }; // Set source rectangle to cover the entire text surface

    SDL_FreeSurface(surf); // Free the surface after texture creation

    // Set the destination and source rectangles in the RenderComponent
    m_RenderComponent->SetDestRect(destRect);
    m_RenderComponent->SetSrcRect(srcRect);
}


// This implementation uses the "dirty flag" pattern
void TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}