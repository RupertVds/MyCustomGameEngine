#pragma once
#include "Observer.h"
#include "TextComponent.h"

class PlayerPointsObserver final : public Component, public Observer
{
public:
	PlayerPointsObserver(GameObject* pOwner);
	~PlayerPointsObserver() = default;
	PlayerPointsObserver(const PlayerPointsObserver& other) = delete;
	PlayerPointsObserver& operator=(const PlayerPointsObserver& other) = delete;
	PlayerPointsObserver(PlayerPointsObserver&& other) = delete;
	PlayerPointsObserver& operator=(PlayerPointsObserver&& other) = delete;
public:
	void Notify(Event event, GameObject* object) override;
private:
	TextComponent* m_pTextComponent;
};