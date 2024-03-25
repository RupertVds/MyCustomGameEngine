#pragma once
#include "Observer.h"
#include "TextComponent.h"

class PlayerLivesObserver final : public Observer
{
public:
	PlayerLivesObserver(TextComponent* text) : Observer(), m_Text{ text } {}
	~PlayerLivesObserver() = default;
	PlayerLivesObserver(const PlayerLivesObserver& other) = delete;
	PlayerLivesObserver& operator=(const PlayerLivesObserver& other) = delete;
	PlayerLivesObserver(PlayerLivesObserver&& other) = delete;
	PlayerLivesObserver& operator=(PlayerLivesObserver&& other) = delete;
public:
	void Notify(Event event, GameObject* object) override;
private:
	// I do this since a gameobject (the player) can't both have a sprite renderer and text render
	// so I have to make another object with the text and reference that object's text component
	TextComponent* m_Text;
};