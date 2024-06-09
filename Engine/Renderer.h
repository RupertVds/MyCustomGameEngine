#pragma once
#include <SDL.h>
#include "Singleton.h"

class Texture2D;
/**
* Simple RAII wrapper for the SDL renderer
*/
class Renderer final : public Singleton<Renderer>
{
public:
	static const int UI_WIDTH{ 96 };
	static const int WIDTH{ 510 + UI_WIDTH };
	static const int HEIGHT{ 400 };
	static const bool FULLSCREEN{ false };
private:
	SDL_Renderer* m_Renderer{};
	SDL_Window* m_Window{};
	SDL_Color m_ClearColor{};	
public:
	void Init(SDL_Window* window);
	void Render() const;
	void Destroy();

	void RenderTexture(const Texture2D& texture, float x, float y) const;
	void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
	void RenderTexture(const Texture2D& texture, const SDL_Rect& srcRect, const SDL_Rect& destRect, bool isFlipped = false) const;

	SDL_Renderer* GetSDLRenderer() const;

	const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
	void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }
};