#pragma once

#include "Component.h"
#include "Texture2D.h"
#include <vector>
#include <string>
#include <memory>

class TilemapComponent : public Component {
public:
    TilemapComponent(GameObject* pOwner, const std::string& collisionDataFile, int tileWidth, int tileHeight);

    void LoadTilemap();
    void Render() const override;

private:
    void LoadFileData(const std::string& fileName, std::vector<std::vector<int>>& data);
    void SetupCollisions();
    void CreateTile(int tileIndex, int row, int col);

    std::vector<std::vector<int>> m_CollisionData;
    std::string m_CollisionDataFile;
    int m_TileWidth;
    int m_TileHeight;
};
