#pragma once
#include "Observer.h"
#include <TextComponent.h>

class PlayerLivesObserver final : public Component, public Observer
{
public:
	PlayerLivesObserver(GameObject* pOwner);
	~PlayerLivesObserver() = default;
	PlayerLivesObserver(const PlayerLivesObserver& other) = delete;
	PlayerLivesObserver& operator=(const PlayerLivesObserver& other) = delete;
	PlayerLivesObserver(PlayerLivesObserver&& other) = delete;
	PlayerLivesObserver& operator=(PlayerLivesObserver&& other) = delete;
private:
	void Notify(Event event, GameObject* object) override;
	TextComponent* m_pTextComponent{};
};