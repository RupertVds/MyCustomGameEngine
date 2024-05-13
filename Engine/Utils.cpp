#include "Utils.h"

RaycastResult Raycast(const glm::vec2& origin, const glm::vec2& direction, float distance, CollisionComponent::ColliderType collisionType)
{
    RaycastResult result;
    result.hit = false;

    // Ray endpoint
    glm::vec2 endPoint = origin + direction * distance;

    // Perform raycast against all colliders
    for (auto& collider : ColliderManager::GetInstance().GetBoxColliders())
    {
        if (collider->GetType() != collisionType)
            continue; // Skip colliders of different type

        glm::vec2 colliderPos = collider->GetPosition();
        float colliderWidth = collider->GetWidth();
        float colliderHeight = collider->GetHeight();

        // Calculate collider vertices
        glm::vec2 vertices[4] = {
            colliderPos,
            colliderPos + glm::vec2(colliderWidth, 0),
            colliderPos + glm::vec2(0, colliderHeight),
            colliderPos + glm::vec2(colliderWidth, colliderHeight)
        };

        // Check for intersection with each edge of the collider
        for (int i = 0; i < 4; ++i) {
            glm::vec2 edgeStart = vertices[i];
            glm::vec2 edgeEnd = vertices[(i + 1) % 4];

            // Calculate normal of the edge
            glm::vec2 edgeDir = glm::normalize(edgeEnd - edgeStart);
            glm::vec2 edgeNormal(edgeDir.y, -edgeDir.x);

            // Calculate intersection point with the edge (if any)
            float t = glm::dot(edgeNormal, (edgeStart - origin)) / glm::dot(edgeNormal, direction);
            if (t >= 0 && t <= distance) {
                glm::vec2 intersection = origin + direction * t;

                // Check if intersection point is inside the edge segment
                float dot1 = glm::dot(edgeEnd - edgeStart, intersection - edgeStart);
                float dot2 = glm::dot(edgeStart - edgeEnd, intersection - edgeEnd);
                if (dot1 >= 0 && dot2 >= 0) {
                    // Intersection found
                    result.hit = true;
                    result.point = intersection;
                    result.normal = edgeNormal;
                    result.distance = t;
                    return result;
                }
            }
        }
    }

    return result;
}