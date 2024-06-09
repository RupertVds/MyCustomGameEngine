#pragma once
#include <string>
#include <vector>

struct HighScoreEntry
{
    std::string playerName;
    int score;
};

class HighScoreSystem
{
public:
    HighScoreSystem();

    void LoadHighScores(const std::string& fileName);
    void SaveHighScores(const std::string& fileName);

    void AddHighScore(const std::string& playerName, int score);
    const std::vector<HighScoreEntry>& GetHighScores() const;

private:
    std::vector<HighScoreEntry> m_HighScores;
};