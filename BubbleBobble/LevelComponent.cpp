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

LevelComponent::LevelComponent(GameObject* pOwner)
    : Component(pOwner), m_CurrentLevel(1) {
    LoadLevel(m_CurrentLevel);
}

void LevelComponent::Render() const {
    // Implement rendering logic if needed
}

void LevelComponent::Update() {
    // Remove enemies marked for deletion from the vector
    m_Enemies.erase(
        std::remove_if(m_Enemies.begin(), m_Enemies.end(), [](GameObject* enemy) {
            return enemy->IsMarkedForDeletion();
            }),
        m_Enemies.end()
    );

    if (AllEnemiesDefeated()) {
        m_CurrentLevel++;
        LoadLevel(m_CurrentLevel);
    }
}

void LevelComponent::LateUpdate() {
    // Implement any late update logic if needed
}

void LevelComponent::LoadLevel(int levelNumber) {
    auto scene = SceneManager::GetInstance().GetSceneByName(GameManager::GetInstance().GetCurrentGameSceneName());

    // Clear previous level objects if needed
    m_Enemies.clear();
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