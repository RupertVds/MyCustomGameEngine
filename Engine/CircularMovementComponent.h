#pragma once
#include "Component.h"
#include "Timer.h"

class CircularMovementComponent final : public Component
{
public:
    CircularMovementComponent(GameObject* pOwner, float radius = 2.f, float speed = 2.f, const glm::vec3& rootPosition = {});
public:
    virtual void Update() override;

private:
    float m_Angle = 0.0f;
    float m_Speed = 15.f;
    float m_Radius = 50.0f;
    glm::vec3 m_RootPosition{};
};