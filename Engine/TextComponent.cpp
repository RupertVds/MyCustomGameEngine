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
	const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
	const auto surf = TTF_RenderText_Blended_Wrapped(m_Font->GetFont(), m_Text.c_str(), color, 9999);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);
	m_RenderComponent->SetTexture(std::make_shared<Texture2D>(texture));
}

// This implementation uses the "dirty flag" pattern
void TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}