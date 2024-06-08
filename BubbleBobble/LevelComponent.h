#pragma once
#include "Component.h"
#include <vector>
#include <string>
#include <memory>
#include "GameObject.h"

class LevelComponent : public Component {
public:
    LevelComponent(GameObject* pOwner);

    virtual void Render() const override;
    virtual void Update() override;
    virtual void LateUpdate() override;

    void LoadLevel(int levelNumber);

    void LoadEnemies(const std::string& fileName);

    void SpawnEnemy(const std::string& name, const glm::vec2& position);

private:
    int m_CurrentLevel;
    std::vector<GameObject*> m_Enemies;  // Store raw pointers to enemies
    GameObject* m_CurrentActiveLevelObject{};
    void SpawnEnemies();
    bool AllEnemiesDefeated() const;
};