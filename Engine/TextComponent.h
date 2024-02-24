#pragma once
#include <string>
#include <memory>
#include "Component.h"
#include "Transform.h"

class Font;
class Texture2D;

class TextComponent : public Component
{
public:
	virtual void Update() override;
	virtual void Render() const override;

	void SetText(const std::string& text);
	void SetPosition(float x, float y);

	TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font);
	virtual ~TextComponent() = default;
	TextComponent(const TextComponent& other) = delete;
	TextComponent(TextComponent&& other) = delete;
	TextComponent& operator=(const TextComponent& other) = delete;
	TextComponent& operator=(TextComponent&& other) = delete;
protected:
	bool m_needsUpdate;
	std::string m_text;
	Transform m_Transform{};
	std::shared_ptr<Font> m_font;
	std::shared_ptr<Texture2D> m_textTexture;
};
