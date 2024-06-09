#pragma once
#include "Component.h"
#include <vector>
#include <string>
#include <memory>
#include "GameObject.h"
#include "Observer.h"

class LevelComponent : public Component, public Observer
{
public:
    LevelComponent(GameObject* pOwner);

    virtual void Render() const override;
    virtual void Update() override;
    virtual void LateUpdate() override;

    void LoadLevel(int levelNumber);
    void SkipLevel();

    void ExitLevel();

    void LoadEnemies(const std::string& fileName);

    void SpawnEnemy(const std::string& name, const glm::vec2& position);
private:
    int m_CurrentLevel;
    const int m_MaxLevel{ 3 };
    std::vector<GameObject*> m_Enemies;  // Store raw pointers to enemies
    GameObject* m_CurrentActiveLevelObject{};
    bool AllEnemiesDefeated() const;

    int m_PlayerOneScore{};
    int m_PlayerTwoScore{};

    // Inherited via Observer
    void Notify(Event event, GameObject* object) override;
};