#pragma once
#include "Observer.h"
#include "TextComponent.h"

class PlayerPointsObserver final : public Observer
{
public:
	PlayerPointsObserver(TextComponent* text) : Observer(), m_Text{ text } {}
	~PlayerPointsObserver() = default;
	PlayerPointsObserver(const PlayerPointsObserver& other) = delete;
	PlayerPointsObserver& operator=(const PlayerPointsObserver& other) = delete;
	PlayerPointsObserver(PlayerPointsObserver&& other) = delete;
	PlayerPointsObserver& operator=(PlayerPointsObserver&& other) = delete;
public:
	void Notify(Event event, GameObject* object) override;
private:
	// I do this since a gameobject (the player) can't both have a sprite renderer and text render
	// so I have to make another object with the text and reference that object's text component
	TextComponent* m_Text;
};