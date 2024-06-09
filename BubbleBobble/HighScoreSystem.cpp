#include "HighScoreSystem.h"
#include <fstream>
#include <algorithm>

HighScoreSystem::HighScoreSystem()
{
    // Initialize or load high scores from a file when the system is created
    LoadHighScores("../Data/highscores.txt");
}

void HighScoreSystem::LoadHighScores(const std::string& fileName)
{
    m_HighScores.clear(); // Clear existing high scores
    std::ifstream file("../Data/" + fileName);
    if (file.is_open())
    {
        std::string playerName;
        int score;
        while (file >> playerName >> score)
        {
            m_HighScores.push_back({ playerName, score });
        }
        file.close();
    }
}

void HighScoreSystem::SaveHighScores(const std::string& fileName)
{
    std::ofstream file("../Data/" + fileName);
    if (file.is_open())
    {
        for (const auto& entry : m_HighScores)
        {
            file << entry.playerName << " " << entry.score << "\n";
        }
        file.close();
    }
}

void HighScoreSystem::AddHighScore(const std::string& playerName, int score)
{
    // Add the new score
    m_HighScores.push_back({ playerName, score });

    // Sort high scores by score in descending order
    std::sort(m_HighScores.begin(), m_HighScores.end(), [](const HighScoreEntry& a, const HighScoreEntry& b) {
        return a.score > b.score;
        });

    // If there are more than 10 scores, remove the lowest one
    if (m_HighScores.size() > 10)
    {
        m_HighScores.pop_back();
    }

    // Save the updated high scores to the file
    SaveHighScores("highscores.txt");
}

const std::vector<HighScoreEntry>& HighScoreSystem::GetHighScores() const
{
    return m_HighScores;
}
