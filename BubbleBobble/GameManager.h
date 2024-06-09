#pragma once
#include "Singleton.h"
#include <string>

class GameManager final : public Singleton<GameManager>
{
public:
	enum class GameState
	{
		TITLESCREEN,
		SINGLEPLAYER,
		MULTIPLAYER,
		VERSUS
	};

public:
	void LoadScene();

	void Render();
	void Update();
	void LateUpdate();

	void SetGameState(GameState state) { m_CurrentState = state; }
	GameState GetGameState() const { return m_CurrentState; }
	const std::string& GetCurrentGameSceneName() const;
private:
	friend class Singleton<GameManager>;
	GameManager() = default;

	void LoadTitleScreen();
	void LoadSinglePlayer();
	void LoadMultiplayer();
	void LoadVersus();

	void LoadGameUI();
	GameState m_CurrentState{ GameState::TITLESCREEN };
	bool m_AddedControllers{};
};

