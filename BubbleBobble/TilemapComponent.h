#pragma once
#include "Tile.h"
#include "Component.h"
#include <vector>

class TilemapComponent : public Component
{
public:
    TilemapComponent(GameObject* pOwner, float tileSize)
        : 
        Component(pOwner),
        m_TileSize{ tileSize }
    {

    }

    ~TilemapComponent();

    // Create a tile at the given row and column indices
    void CreateTile(int row, int col)
    {
        float x = col * m_TileSize;
        float y = row * m_TileSize;
        m_Tiles.emplace_back(std::make_unique<Tile>(x, y, m_TileSize, m_TileSize));
    }

    // Render method to draw all tiles
    void Render() const override
    {
        for (const auto& tile : m_Tiles) {
            tile->Render();
        }
    }

    void DestroyTiles() { m_Tiles.clear(); }

private:
    std::vector<std::unique_ptr<Tile>> m_Tiles; // 2D array of tiles
    float m_TileSize; // Size of each tile
};