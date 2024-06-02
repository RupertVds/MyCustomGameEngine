#pragma once
#include "Component.h"
#include "Transform.h"
#include <string>
#include <memory>
#include "RenderComponent.h"

class Font;
class Texture2D;

class TextComponent : public Component
{
public:
	TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font);
public:
	virtual void Update() override;
	//virtual void Render() const override;

	void SetText(const std::string& text);
private:
	void UpdateTexture();
protected:
	bool m_NeedsUpdate;
	std::string m_Text;
	std::shared_ptr<Font> m_Font;
	RenderComponent* m_RenderComponent;
};
