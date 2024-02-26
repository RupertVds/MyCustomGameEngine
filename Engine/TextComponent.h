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
	//=======================================
	// RULE OF ZERO/FIVE/SIX
	//=======================================
	TextComponent(const std::string& text, std::shared_ptr<Font> font);
	virtual ~TextComponent() = default;
	TextComponent(const TextComponent& other) = delete;
	TextComponent(TextComponent&& other) = delete;
	TextComponent& operator=(const TextComponent& other) = delete;
	TextComponent& operator=(TextComponent&& other) = delete;
public:
	virtual void Update() override;
	virtual void Render() const override;

	void SetText(const std::string& text);
protected:
	bool m_NeedsUpdate;
	std::string m_Text;
	std::shared_ptr<Font> m_Font;
	std::shared_ptr<Texture2D> m_TextTexture;
};
