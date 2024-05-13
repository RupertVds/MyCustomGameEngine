#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "ColliderManager.h"

struct RaycastResult 
{
    bool hit;
    glm::vec2 point;
    glm::vec2 normal;
    float distance;
};

RaycastResult Raycast(const glm::vec2& origin, const glm::vec2& direction, float distance, CollisionComponent::ColliderType collisionType);