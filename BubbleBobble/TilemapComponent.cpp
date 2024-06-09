#include "TilemapComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "BoxColliderComponent.h"
#include "RenderComponent.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

TilemapComponent::TilemapComponent(GameObject* pOwner, const std::string& collisionDataFile, int tileWidth, int tileHeight)
    : Component(pOwner),
    m_CollisionDataFile(collisionDataFile),
    m_TileWidth(tileWidth),
    m_TileHeight(tileHeight)
{
    LoadTilemap();
}

void TilemapComponent::LoadTilemap() 
{
    //LoadFileData(m_VisualDataFile, m_TileData);
    LoadFileData(m_CollisionDataFile, m_CollisionData);
    SetupCollisions();
}

void TilemapComponent::LoadFileData(const std::string& fileName, std::vector<std::vector<int>>& data)
{
    // Clear the data vector to ensure no previous data is left
    data.clear();

    std::string filePath = "../Data/" + fileName;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open data file: " + filePath);
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::vector<int> row;
        std::istringstream iss(line);
        int value;
        while (iss >> value) {
            row.push_back(value);
        }
        if (!row.empty()) {
            data.push_back(row);
        }

        // Debug output to check the values being read
        //std::cout << "Loaded row: ";
        //for (int val : row) {
        //    std::cout << val << " ";
        //}
        //std::cout << std::endl;
    }

    file.close();

    // Debug output to check the entire data
    //std::cout << "Loaded data from " << fileName << ":" << std::endl;
    //for (const auto& row : data) {
    //    for (int val : row) {
    //        std::cout << val << " ";
    //    }
    //    std::cout << std::endl;
    //}
}


void TilemapComponent::Render() const 
{
}

void TilemapComponent::SetupCollisions() {
    int rows = static_cast<int>(m_CollisionData.size());
    int cols = rows > 0 ? static_cast<int>(m_CollisionData[0].size()) : 0;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            //int tileIndex = m_TileData[row][col];
            int collisionType = m_CollisionData[row][col];
            //if (tileIndex > 0) {
            //    CreateTile(tileIndex, row, col);
            //}
            if (collisionType > 0) {
                auto collisionTile = std::make_unique<GameObject>();
                collisionTile->SetLocalPosition({ col * m_TileWidth, row * m_TileHeight });
                collisionTile->AddComponent<BoxColliderComponent>(static_cast<float>(m_TileWidth), static_cast<float>(m_TileHeight), CollisionComponent::ColliderType::STATIC);
                GetOwner()->AddChild(std::move(collisionTile));
            }
        }
    }
}