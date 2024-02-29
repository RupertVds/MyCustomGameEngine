#pragma once
#include "Component.h"
#include "Timer.h"

class TextComponent;

class FPSComponent final : public Component
{
public:
    FPSComponent(GameObject* pOwner);
public:
    virtual void Update() override;

private:
    TextComponent* m_TextComponent;
    int m_FrameCount = 0;
    float m_ElapsedTime = 0.0f;
};
