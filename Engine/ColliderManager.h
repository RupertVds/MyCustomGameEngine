#pragma once
#include "Singleton.h"
#include <vector>
#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"

class BoxColliderComponent;
class CircleColliderComponent;

class ColliderManager final : public Singleton<ColliderManager>
{
public:
    void RegisterBoxCollider(BoxColliderComponent* collider) {
        m_BoxColliders.push_back(collider);
    }

    void UnregisterBoxCollider(BoxColliderComponent* collider) {
        auto it = std::find(m_BoxColliders.begin(), m_BoxColliders.end(), collider);
        if (it != m_BoxColliders.end()) {
            m_BoxColliders.erase(it);
        }
    }

    void RegisterCircleCollider(CircleColliderComponent* collider) {
        m_CircleColliders.push_back(collider);
    }

    void UnregisterCircleCollider(CircleColliderComponent* collider) {
        auto it = std::find(m_CircleColliders.begin(), m_CircleColliders.end(), collider);
        if (it != m_CircleColliders.end()) {
            m_CircleColliders.erase(it);
        }
    }

    const std::vector<BoxColliderComponent*>& GetBoxColliders() const {
        return m_BoxColliders;
    }

    const std::vector<CircleColliderComponent*>& GetCircleColliders() const {
        return m_CircleColliders;
    }

private:
    friend class Singleton<ColliderManager>;
    ColliderManager() = default;
    std::vector<BoxColliderComponent*> m_BoxColliders;
    std::vector<CircleColliderComponent*> m_CircleColliders;
};
