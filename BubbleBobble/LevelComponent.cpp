#include "LevelComponent.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "ZenChanComponent.h"
#include "TilemapComponent.h"
#include "RenderComponent.h"
#include "BoxColliderComponent.h"
#include "AnimatorComponent.h"
#include <Renderer.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "PlayerComponent.h"

LevelComponent::LevelComponent(GameObject* pOwner)
    : Component(pOwner), m_CurrentLevel(1) {
    LoadLevel(m_CurrentLevel);
}

void LevelComponent::Render() const {
    // Implement rendering logic if needed
}

void LevelComponent::Update() {
}

void LevelComponent::LateUpdate() {
    // Remove enemies marked for deletion from the vector
    m_Enemies.erase(
        std::remove_if(m_Enemies.begin(), m_Enemies.end(), [](GameObject* enemy) {
            return enemy->IsMarkedForDeletion();
            }),
        m_Enemies.end()
    );

    if (AllEnemiesDefeated()) {
        m_CurrentLevel++;

        if (m_CurrentLevel > m_MaxLevel)
        {
            ExitLevel();
            return;
        }

        LoadLevel(m_CurrentLevel);
    }
}

void LevelComponent::LoadLevel(int levelNumber) {
    auto scene = SceneManager::GetInstance().GetSceneByName(GameManager::GetInstance().GetCurrentGameSceneName());

    // Clear previous level objects if needed
    m_Enemies.clear();
    scene->DeleteObjectsByName("watermelon");
    scene->DeleteObjectsByName("bubble");
    scene->DeleteObjectsByName("ZenChan");

    if(m_CurrentActiveLevelObject) m_CurrentActiveLevelObject->DeleteSelf();

    std::string levelCollisionsFile = "level_" + std::to_string(levelNumber) + "_collisions.txt";
    std::string levelTextureFile = "level_" + std::to_string(levelNumber) + ".png";
    std::string enemyFile = "level_" + std::to_string(levelNumber) + "_enemies.txt";

    // Load new level
    auto levelObject = std::make_unique<GameObject>();
    levelObject->AddComponent<TilemapComponent>(levelCollisionsFile, 16, 16);
    levelObject->AddComponent<RenderComponent>(ResourceManager::GetInstance().LoadTexture(levelTextureFile));

    m_CurrentActiveLevelObject = levelObject.get();

    scene->Add(std::move(levelObject));

    // Spawn enemies
    LoadEnemies(enemyFile);
}

void LevelComponent::SkipLevel()
{
    ++m_CurrentLevel;
    if (m_CurrentLevel > m_MaxLevel)
    {
        std::cout << "FINAL LEVEL COMPLETED\n";
        ExitLevel();
        return;
    }

    LoadLevel(m_CurrentLevel);
}

void LevelComponent::ExitLevel()
{
    GameManager::GetInstance().SetGameState(GameManager::GameState::HIGHSCORES);
    SceneManager::GetInstance().DestroyAllScenes();
    GameManager::GetInstance().LoadScene();

    // Final level completed, update high scores
    HighScoreSystem& highScoreSystem = GameManager::GetInstance().GetHighScoreSystem();
    highScoreSystem.AddHighScore(GameManager::GetInstance().GetPlayerOneName(), m_PlayerOneScore);
    highScoreSystem.AddHighScore(GameManager::GetInstance().GetPlayerTwoName(), m_PlayerTwoScore);
    highScoreSystem.SaveHighScores("highscores.txt");
    highScoreSystem.LoadHighScores("highscores.txt");
}

void LevelComponent::LoadEnemies(const std::string& fileName) {
    std::ifstream file("../Data/" + fileName);
    if (!file.is_open()) {
        std::cerr << "Failed to open enemy file: " << fileName << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string enemyName;
        float offsetX, posY;
        if (!(iss >> enemyName >> offsetX >> posY)) {
            std::cerr << "Invalid line format in enemy file: " << line << std::endl;
            continue;
        }

        glm::vec2 position = { (Renderer::WIDTH - Renderer::UI_WIDTH) / 2 - 8 + offsetX, posY };
        SpawnEnemy(enemyName, position);
    }
}

void LevelComponent::SpawnEnemy(const std::string& name, const glm::vec2& position) {
    auto scene = SceneManager::GetInstance().GetSceneByName(GameManager::GetInstance().GetCurrentGameSceneName());

    auto zenChanRunTexture = ResourceManager::GetInstance().LoadTexture("ZenChan/Run_Anim.png");
    auto zenChanDeadBubbleTexture = ResourceManager::GetInstance().LoadTexture("ZenChan/Bubble_Anim.png");
    auto zenChanDeadTexture = ResourceManager::GetInstance().LoadTexture("ZenChan/Death_Anim.png");

    auto enemyObject = std::make_unique<GameObject>(name);
    enemyObject->AddComponent<RenderComponent>();
    auto enemyAnimator = enemyObject->AddComponent<AnimatorComponent>();
    enemyAnimator->AddSpriteSheet("DeadBubble", zenChanDeadBubbleTexture, 16, 16, 8);
    enemyAnimator->AddSpriteSheet("Dead", zenChanDeadTexture, 16, 16, 8);
    enemyAnimator->AddSpriteSheet("Run", zenChanRunTexture, 16, 16, 8);
    enemyObject->AddComponent<BoxColliderComponent>(24.f, 32.f, CollisionComponent::ColliderType::DYNAMIC);
    enemyObject->AddComponent<ZenChanComponent>();

    enemyObject->SetScale({ 2.f, 2.f, 2.f });
    enemyObject->SetLocalPosition(position);

    GameObject* enemyRawPtr = enemyObject.get();  // Store raw pointer
    m_Enemies.push_back(enemyRawPtr);
    scene->Add(std::move(enemyObject));  // Move unique pointer to scene
}

bool LevelComponent::AllEnemiesDefeated() const {
    // Check if all enemies are nullptr
    // Check if the enemy vector is empty
    return m_Enemies.empty();
}

void LevelComponent::Notify(Event event, GameObject* object)
{
    switch (event)
    {
    case Event::PLAYER_JOIN:
        std::cout << "LEVEL: PLAYER JOINED DETECTED\n";
        break;
    case Event::PLAYER_DAMAGE:
        break;
    case Event::PLAYER_DIED:
        ExitLevel();
        break;
    case Event::PLAYER_SCORE:
        break;
    case Event::WATERMELON_PICKUP:
        std::cout << "LEVEL: WATERMELON_PICKUP DETECTED\n";
        if (object->GetName() == "player_1")
        {
            // we are safe to assume it has a player comp
            PlayerComponent* playerOneComp = object->GetComponent<PlayerComponent>();
            if (playerOneComp)
            {
                playerOneComp->GetOwner()->NotifyObservers(Event::PLAYER_SCORE);
                m_PlayerOneScore = playerOneComp->GetScore();
            }
        }
        else if (object->GetName() == "player_2")
        {
            // we are safe to assume it has a player comp
            PlayerComponent* playerTwoComp = object->GetComponent<PlayerComponent>();
            if (playerTwoComp)
            {
                playerTwoComp->GetOwner()->NotifyObservers(Event::PLAYER_SCORE);
                m_PlayerTwoScore = playerTwoComp->GetScore();
            }
        }
        break;
    default:
        break;
    }
    object;
}
