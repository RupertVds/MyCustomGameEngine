#pragma once
#include "Component.h"
#include "Timer.h"

class CircularMovementComponent final : public Component
{
public:
    CircularMovementComponent(GameObject* pOwner, float radius = 2.f, float speed = 2.f);
public:
    virtual void Update() override;

private:
    float m_Angle{};
    float m_Speed{};
    float m_Radius{};
};