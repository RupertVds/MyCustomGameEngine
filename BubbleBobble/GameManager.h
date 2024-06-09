#pragma once
#include "Singleton.h"
#include <string>
#include "HighScoreSystem.h"

class GameManager final : public Singleton<GameManager>
{
public:
	enum class GameState
	{
		TITLESCREEN,
		SINGLEPLAYER,
		MULTIPLAYER,
		VERSUS,
		HIGHSCORES
	};

public:
	void LoadScene();

	void Render();
	void Update();
	void LateUpdate();

	void SetGameState(GameState state) { m_CurrentState = state; }
	void SetPlayerOneName(const std::string& name) { m_PlayerOneName = name; }
	const std::string& GetPlayerOneName() { return m_PlayerOneName; }
	void SetPlayerTwoName(const std::string& name) { m_PlayerTwoName = name; }
	const std::string& GetPlayerTwoName() { return m_PlayerTwoName; }
	GameState GetGameState() const { return m_CurrentState; }
	const std::string& GetCurrentGameSceneName() const;

	HighScoreSystem& GetHighScoreSystem() { return m_HighScoreSystem; }
private:
	friend class Singleton<GameManager>;
	GameManager() = default;

	void LoadTitleScreen();
	void LoadSinglePlayer();
	void LoadMultiplayer();
	void LoadVersus();

	void LoadHighScores();

	GameState m_CurrentState{ GameState::TITLESCREEN };
	bool m_AddedControllers{};

	std::string m_PlayerOneName{};
	std::string m_PlayerTwoName{};

	HighScoreSystem m_HighScoreSystem;
};

