#pragma once
#include "Component.h"
#include "BoxColliderComponent.h"
#include <vector>

class TilemapComponent : public Component
{
public:
    TilemapComponent(GameObject* pOwner, float tileSize)
        : Component(pOwner), m_TileSize(tileSize)
    {}

    ~TilemapComponent()
    {
        DestroyTiles();
    }

    // Create a tile at the given row and column indices
    void CreateTile(int row, int col)
    {
        float x = col * m_TileSize;
        float y = row * m_TileSize;

        // Create GameObject for the tile
        GameObject* tileObject = new GameObject(); // You may need to manage the memory of these objects
        tileObject->SetLocalPosition({ x, y, 0 }); // Set position based on row and col
        //tileObject->SetSize({ m_TileSize, m_TileSize }); // Set size of the tile
        tileObject->AddComponent<BoxColliderComponent>(m_TileSize, m_TileSize, CollisionComponent::ColliderType::STATIC); // Add BoxColliderComponent

        // Add the tile GameObject to the owner of TilemapComponent
        //GetOwner()->AddChild(tileObject);
        //tileObject->SetParent(GetOwner()->shared_from_this());
    }

    // Destroy all tiles (tile GameObjects)
    void DestroyTiles()
    {
        //// Destroy each tile GameObject
        //for (auto& child : GetOwner()->GetChildren())
        //{
        //    delete child; // You may need to manage the memory of these objects
        //}
        //GetOwner()->ClearChildren(); // Clear the children list
    }

private:
    float m_TileSize; // Size of each tile
};
